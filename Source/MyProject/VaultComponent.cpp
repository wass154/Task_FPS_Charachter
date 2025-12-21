#include "VaultComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


UVaultComponent::UVaultComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVaultComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeVault();
}

void UVaultComponent::InitializeVault()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "OwnerCharacter Now ...");
	}
	if (!OwnerCharacter) return;

	Capsule = OwnerCharacter->GetCapsuleComponent();
	Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
}

/* ================= TRY VAULT ================= */

void UVaultComponent::TryVault()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "TryVault Now ...");
	}

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryVault: OwnerCharacter is null!"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "TryVault: OwnerCharacter is null Now ...");
		return;
	}

	if (bIsTraversing)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryVault: Already traversing!"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "TryVault: Already traversing Now ...");
		return;
	}

	FHitResult Hit;
	float Height = 0.f;
	float Depth = 0.f;

	if (!CheckVaultableObstacle(Hit, Height, Depth)) return;

	CurrentVaultType = DecideVaultType(Height, Depth);
	if (CurrentVaultType == EVault::None) return;

	bIsTraversing = true;

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->DisableMovement();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartLocation = OwnerCharacter->GetActorLocation();

	if (Camera)
	{
		CameraStartOffset = Camera->GetRelativeLocation();
		CameraVaultOffset = CameraStartOffset + FVector(30.f, 0.f, 20.f);
		bCameraOffsetActive = true;
	}

	if (CurrentVaultType == EVault::Vaulting)
	{
		TargetLocation =
			Hit.ImpactPoint + OwnerCharacter->GetActorForwardVector() * (Capsule->GetScaledCapsuleRadius() + 50.f) + FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight() + 10.f);
		
	
	}
	else if (CurrentVaultType == EVault::Mantling)
	{
		TargetLocation = StartLocation + OwnerCharacter->GetActorForwardVector() * 60.f + FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight() + 40.f);	
		
	}
}

/* ================= DECISION ================= */

EVault UVaultComponent::DecideVaultType(float Height, float Depth) const
{
	if (Height > MaxVaultHeight)
		return EVault::Ledging;

	// Vault if height and depth allow
	if (Height >= MinHeight && Height <= MaxVaultHeight && Depth <= MaxVaultDepth)
		return EVault::Vaulting;

	// Mantle if obstacle is high enough to require mantling and depth is big
	if (Height >= MaxVaultHeight && Height <= MaxMantleHeight)
		return EVault::Mantling;

	return EVault::None;
}

/* ================= TRACE ================= */

bool UVaultComponent::CheckVaultableObstacle(FHitResult& OutHit, float& OutHeight, float& OutDepth, EVaultType& OutVaultType)
{
	if (!OwnerCharacter || !Capsule) return false;

	// 1. SETUP DATA
	FVector CharacterLocation = OwnerCharacter->GetActorLocation();
	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector Down = FVector::DownVector;
	float CapsuleBaseZ = CharacterLocation.Z - Capsule->GetScaledCapsuleHalfHeight();

	// ---------------------------------------------------------
	// 2. WALL TRACE (Forward) - Find the wall
	// ---------------------------------------------------------
	const FVector Start = CharacterLocation;
	const FVector End = Start + (Forward * TraceDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	FHitResult WallHit;
	// Sweep a small sphere forward to find the wall
	bool bHitWall = GetWorld()->SweepSingleByChannel(
		WallHit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(10.0f),
		Params
	);

	if (!bHitWall || !WallHit.GetActor())
	{
		return false;
	}

	// ---------------------------------------------------------
	// 3. HEIGHT TRACE (Downward) - Find the top of the wall
	// ---------------------------------------------------------
	// We move forward slightly past the wall hit, and trace down from above
	FVector HeightStart = WallHit.Location + (Forward * 15.0f) + FVector(0, 0, MaxMantleHeight + 50.0f);
	FVector HeightEnd = WallHit.Location + (Forward * 15.0f); // Trace to level of impact

	FHitResult HeightHit;
	// Trace down to find the surface
	bool bHitTop = GetWorld()->LineTraceSingleByChannel(HeightHit, HeightStart, HeightEnd, ECC_Visibility, Params);

	DrawDebugLine(GetWorld(), HeightStart, HeightEnd, FColor::Orange, false, 2.0f);

	if (!bHitTop)
	{
		// We hit a wall, but nothing flat on top (maybe a slope or ceiling?)
		return false;
	}

	// CALCULATE HEIGHT
	// Height is the Top Surface Z minus the Player's Feet Z
	OutHeight = HeightHit.ImpactPoint.Z - CapsuleBaseZ;

	// ---------------------------------------------------------
	// 4. DEPTH TRACE - Check thickness
	// ---------------------------------------------------------
	// We check a point further ahead (e.g., 100 units or your '10' threshold)
	// If we trace down there and hit nothing, it's a thin object (Vault).
	// If we hit something, it's a thick platform.

	FVector DepthCheckStart = HeightHit.ImpactPoint + (Forward * 15.0f); // Check 15 units past the front edge
	FVector DepthCheckEnd = DepthCheckStart - FVector(0, 0, 50.0f);     // Small check down

	FHitResult DepthHit;
	bool bHitBack = GetWorld()->LineTraceSingleByChannel(DepthHit, DepthCheckStart, DepthCheckEnd, ECC_Visibility, Params);

	// If we hit back, the depth is large (at least 15). If we missed, it's thin.
	// Ideally, you perform multiple traces or one long trace to find exact depth, 
	// but here we simply check if it exists past the threshold.
	OutDepth = bHitBack ? 50.0f : 5.0f; // Simplified: 50 = Thick, 5 = Thin

	DrawDebugLine(GetWorld(), DepthCheckStart, DepthCheckEnd, FColor::Cyan, false, 2.0f);

	// ---------------------------------------------------------
	// 5. CLASSIFICATION LOGIC (User Rules)
	// ---------------------------------------------------------

	// Default to false/invalid
	OutVaultType = EVaultType::None; // Assuming you have an Enum for this
	bool bIsVaultable = false;

	// Logic: Height < 20 is Vault
	if (OutHeight < 20.0f)
	{
		OutVaultType = EVaultType::Vault;
		bIsVaultable = true;
	}
	// Logic: Height < 50 is Mantle
	else if (OutHeight < 50.0f)
	{
		OutVaultType = EVaultType::Mantle;
		bIsVaultable = true;
	}
	// Logic: Height > 50 is Ledge (Climb)
	else
	{
		OutVaultType = EVaultType::Ledge; // Or Climb
		bIsVaultable = true;
	}

	// Logic: Depth Override 
	// "If depth < 10 is vault, if > 10 is mantle"
	// We check if we missed the back trace (implying depth is very small/thin)
	if (!bHitBack)
	{
		// Thin fence or railing -> Force Vault
		OutVaultType = EVaultType::Vault;
	}

	// Logging for Debug
	UE_LOG(LogTemp, Warning, TEXT("H: %.2f | D: %s | Type: %d"),
		OutHeight,
		bHitBack ? TEXT(">10") : TEXT("<10"),
		(int32)OutVaultType);

	OutHit = WallHit; // Return the wall hit for location data
	return bIsVaultable;
}


/* ================= TICK ================= */

void UVaultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ApplyTraversal(DeltaTime);
}

/* ================= APPLY ================= */

void UVaultComponent::ApplyTraversal(float DeltaTime)
{
	if (!bIsTraversing) return;

	if (CurrentVaultType == EVault::Vaulting)
		PerformVault(DeltaTime);
	else if (CurrentVaultType == EVault::Mantling)
		PerformMantle(DeltaTime);
}

/* ================= MOVE ================= */

void UVaultComponent::PerformVault(float DeltaTime)
{
	MoveCharacter(DeltaTime, InterpSpeed);
}

void UVaultComponent::PerformMantle(float DeltaTime)
{
	MoveCharacter(DeltaTime, InterpSpeed * 0.8f);
}

void UVaultComponent::MoveCharacter(float DeltaTime, float Speed)
{
	FVector NewLoc = FMath::VInterpTo(OwnerCharacter->GetActorLocation(), TargetLocation, DeltaTime, Speed);

	FHitResult Hit;
	OwnerCharacter->SetActorLocation(NewLoc, true, &Hit, ETeleportType::TeleportPhysics);

	ApplyCameraOffset(DeltaTime);

	if (FVector::Dist(NewLoc, TargetLocation) < 5.f)
	{
		FinishTraversal();
	}
}

/* ================= CAMERA ================= */

void UVaultComponent::ApplyCameraOffset(float DeltaTime)
{
	if (!Camera || !bCameraOffsetActive) return;

	// Base target offset (linear)
	FVector BaseTargetOffset = bIsTraversing ? CameraVaultOffset : CameraStartOffset;

	// Add a subtle vertical curve for mantling
	FVector CurrentOffset = Camera->GetRelativeLocation();
	FVector OffsetDifference = BaseTargetOffset - CameraStartOffset;

	FVector CurvedOffset = BaseTargetOffset;


	// Create a smooth "up then forward" curve based on distance
	float TotalDistance = FVector::Dist(StartLocation, TargetLocation);
	float CurrentDistance = FVector::Dist(OwnerCharacter->GetActorLocation(), TargetLocation);

	// Ratio goes from 1 -> 0 as character approaches target
	float Ratio = FMath::Clamp(CurrentDistance / TotalDistance, 0.f, 1.f);

	// Apply vertical offset curve using sine for smooth up/down
	float VerticalOffset = OffsetDifference.Z * FMath::Sin((1 - Ratio) * PI * 0.5f); // goes up smoothly
	CurvedOffset.Z = CameraStartOffset.Z + VerticalOffset;
	

	// Smoothly interpolate using VInterpTo for cinematic feel
	FVector NewOffset = FMath::VInterpTo(CurrentOffset, CurvedOffset, DeltaTime, CameraInterpSpeed);
	Camera->SetRelativeLocation(NewOffset);
}

/* ================= FINISH ================= */

void UVaultComponent::FinishTraversal()
{
	bIsTraversing = false;
	bCameraOffsetActive = false;
	CurrentVaultType = EVault::None;

	if (Camera)
		Camera->SetRelativeLocation(CameraStartOffset);

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

bool UVaultComponent::IsVaulting() const
{
	return bIsTraversing;
}







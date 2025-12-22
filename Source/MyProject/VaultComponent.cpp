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
	if (Height > MaxMantleHeight)
	{
		return EVault::Ledging;
	}


	if (Height > MaxVaultHeight && Height <= MaxMantleHeight)
	{
		return EVault::Mantling;
	}


	if (Height >= MinHeight && Height <= MaxVaultHeight && Depth <= MaxVaultDepth)
	{
		return EVault::Vaulting;
	}

	return EVault::None;
}

/* ================= TRACE ================= */

bool UVaultComponent::CheckVaultableObstacle(
	FHitResult& Hit,
	float& OutHeight,
	float& OutDepth
)
{
	if (!OwnerCharacter || !Capsule)
		return false;

	UWorld* World = GetWorld();
	if (!World)
		return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	const FVector ActorLocation = OwnerCharacter->GetActorLocation();
	const FVector ActorForward = OwnerCharacter->GetActorForwardVector();

	/* =====================================================
	   1) FIRST TRACE – FIND WALL
	   ===================================================== */

	const FVector WallStart = ActorLocation + FVector(0.f, 0.f, MinHeight);
	const FVector WallEnd = WallStart + ActorForward * TraceDistance;

	bool bWallHit = World->SweepSingleByChannel(
		Hit,
		WallStart,
		WallEnd,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

	DrawDebugLine(World, WallStart, WallEnd, FColor::Green, false, 1.5f, 0, 2.f);

	if (!bWallHit)
		return false;

	/* =====================================================
	   2) WALL NORMAL → WALL FORWARD
	   ===================================================== */

	const FVector WallNormal = Hit.ImpactNormal.GetSafeNormal();
	const FVector WallForward = (-WallNormal).GetSafeNormal();

	// Push inside wall to avoid missing top edge
	const FVector WallPoint = Hit.ImpactPoint + WallForward * 5.f;

	/* =====================================================
	   3) TOP TRACE – FIND WALL TOP
	   ===================================================== */

	FHitResult TopHit;

	const FVector TopTraceStart = WallPoint + FVector(0.f, 0.f, 900.f);
	const FVector TopTraceEnd = WallPoint - FVector(0.f, 0.f, 900.f);

	bool bTopHit = World->LineTraceSingleByChannel(
		TopHit,
		TopTraceStart,
		TopTraceEnd,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(World, TopTraceStart, TopTraceEnd, FColor::Yellow, false, 1.5f, 0, 2.f);

	if (!bTopHit)
		return false;

	/* =====================================================
	   4) HEIGHT = TOP HIT − WALL HIT (Z ONLY)
	   ===================================================== */

	OutHeight = TopHit.ImpactPoint.Z - Hit.ImpactPoint.Z;
	VaultHeight = OutHeight;

	/* =====================================================
	   5) DEPTH TRACE (OPTIONAL)
	   ===================================================== */

	   /* =====================================================
		  5) DEPTH TRACE – LANDING SPACE
		  ===================================================== */

	FHitResult DepthHit;

	const FVector DepthStart =
		TopHit.ImpactPoint
		+ FVector(0.f, 0.f, 20.f)   // above top surface
		+ WallForward * 30.f;       // past wall edge

	const FVector DepthEnd =
		DepthStart - FVector(0.f, 0.f, 350.f);

	bool bDepthHit = World->LineTraceSingleByChannel(
		DepthHit,
		DepthStart,
		DepthEnd,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(World, DepthStart, DepthEnd, FColor::Blue, false, 1.5f, 0, 2.f);

	OutDepth = bDepthHit
		? FVector::Dist2D(DepthStart, DepthHit.ImpactPoint)
		: MaxVaultDepth;

	VaultDepth = OutDepth;

	/* =====================================================
	   6) DEBUG
	   ===================================================== */

	DrawDebugString(
		World,
		Hit.ImpactPoint + FVector(0.f, 0.f, 80.f),
		FString::Printf(TEXT("H: %.1f  D: %.1f"), OutHeight, OutDepth),
		nullptr,
		FColor::Red,
		1.5f,
		true
	);

	/* =====================================================
	   7) VALIDATE HEIGHT
	   ===================================================== */

	return OutHeight >= MinHeight && OutHeight <= MaxMantleHeight;
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







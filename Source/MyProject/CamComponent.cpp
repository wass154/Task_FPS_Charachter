// Fill out your copyright notice in the Description page of Project Settings.


#include "CamComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "MyTraversalCharachter.h"


// Sets default values for this component's properties
UCamComponent::UCamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

#pragma region Intialization
void UCamComponent::InitializeCharachter()
{
	OwningCharacter = Cast<ACharacter>(GetOwner());

	if (!OwningCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("GorgeousCameraComponent is not attached to an ACharacter! %s"), *GetName()));
		return;
	}

	PlayerController = Cast<APlayerController>(OwningCharacter->GetController());
	if (PlayerController)
	{
		CameraManager = PlayerController->PlayerCameraManager;
	}


	TraversalCharachter = Cast<AMyTraversalCharachter>(OwningCharacter);
	if (TraversalCharachter)
	{
		
		CameraBoomRef = TraversalCharachter->GetCameraBoom();
		FollowCameraRef = TraversalCharachter->GetCameraFollow();
	}
}

void UCamComponent::InitializeCam()
{
	// FPS settings
	OwningCharacter->bUseControllerRotationYaw = true;
	OwningCharacter->bUseControllerRotationPitch = false;
	OwningCharacter->bUseControllerRotationRoll = false;

	if (OwningCharacter->GetCharacterMovement())
	{
		OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	// Clamp pitch in PlayerCameraManager
	if (CameraManager)
	{
		CameraManager->ViewPitchMin = MinPitch;
		CameraManager->ViewPitchMax = MaxPitch;
	}

	if (FollowCameraRef)
	{
		CameraDefaultLocation = FollowCameraRef->GetRelativeLocation();
		CurrentFOV = FollowCameraRef->FieldOfView;
	}
}
void UCamComponent::UpdateCameraPosition(float DeltaTime)
{
	if (!FollowCameraRef || !OwningCharacter) return;

	// Target camera position (e.g., character's head or capsule location)
	FVector TargetLocation = OwningCharacter->GetActorLocation() + FVector(0, 0, OwningCharacter->BaseEyeHeight);

	// Smoothly interpolate
	FVector NewLocation = FMath::VInterpTo(FollowCameraRef->GetComponentLocation(), TargetLocation, DeltaTime, 12.f);
	FollowCameraRef->SetWorldLocation(NewLocation);
}
void UCamComponent::UpdateCameraRotation(float DeltaTime)
{
	if (!FollowCameraRef || !PlayerController) return;


	// Update desired rotation
	FRotator TargetRotation = FollowCameraRef->GetRelativeRotation();
	TargetRotation.Yaw += PlayerController->GetControlRotation().Roll * TurnRate * MouseSensitivity * DeltaTime;
	TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch + PlayerController->GetControlRotation().Yaw * LookUpRate * MouseSensitivity * DeltaTime, MinPitch, MaxPitch);

	// Smoothly interpolate rotation
	FRotator NewRotation = FMath::RInterpTo(FollowCameraRef->GetRelativeRotation(), TargetRotation, DeltaTime, SmoothSpeed);
	FollowCameraRef->SetRelativeRotation(NewRotation);
}
#pragma endregion Intialization

#pragma region CameraFunctional
void UCamComponent::LookUp(float Value)
{
	if (OwningCharacter && Value != 0.f)
		OwningCharacter->AddControllerPitchInput(Value * LookUpRate * MouseSensitivity * GetWorld()->GetDeltaSeconds());
}

void UCamComponent::Turn(float Value)
{
	if (OwningCharacter && PlayerController && !FMath::IsNearlyZero(Value))
		OwningCharacter->AddControllerYawInput(Value * TurnRate * MouseSensitivity * GetWorld()->GetDeltaSeconds());
}
void UCamComponent::Lean(float Value)
{
	if (!CameraBoomRef) return;

	TargetLean = FMath::Clamp(Value, -1.f, 1.f) * MaxLeanAngle;
}
void UCamComponent::ApplyLean(float DeltaTime)
{
	// Smooth Lean
	if (!FollowCameraRef) return;	
	// Smoothly interpolate lean
	CurrentLean = FMath::FInterpTo(CurrentLean, TargetLean, DeltaTime, LeanSmoothSpeed);

	// Apply lean directly to camera roll
	FRotator CamRot = FollowCameraRef->GetRelativeRotation();
	CamRot.Roll = CurrentLean; // Only roll, pitch and yaw remain unchanged
	FollowCameraRef->SetRelativeRotation(CamRot);
}
float UCamComponent::GetTargetFOV() const
{
	if (!TraversalCharachter) return IdleFOV;

	//if (TraversalCharachter->IsVaulting() || TraversalCharachter->IsMantling())
	//	return VaultFOV;

	//if (TraversalCharachter->IsSliding())
	//	return SlideFOV;

	if (TraversalCharachter->MainComponents->CrouchComp->bIsCrouching)
		return CrouchFOV;

	const float Speed = TraversalCharachter->GetVelocity().Size2D();

	// Idle
	if (Speed < 10.f)
		return IdleFOV;

	// Run
	if (Speed > 300.f)
		return RunFOV;

	// Walk
	return WalkFOV;
}
void UCamComponent::UpdateFOV(float DeltaTime)
{
	if (!FollowCameraRef) return;

	TargetFOV = GetTargetFOV();
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
	FollowCameraRef->SetFieldOfView(CurrentFOV);
}
void UCamComponent::UpdateTilt(float DeltaTime)
{
	if (!FollowCameraRef || !OwningCharacter) return;

	// Get the control rotation (from player controller)
	FRotator ControlRot = OwningCharacter->GetController()->GetControlRotation();

	// Calculate target tilt
	// Tilt Roll based on yaw (left/right)
	float TargetRoll = FMath::Clamp(ControlRot.Yaw * TiltYawFactor, -MaxTiltAngle, MaxTiltAngle);

	// Optional: Tilt Pitch slightly based on looking up/down
	float TargetPitchTilt = FMath::Clamp(ControlRot.Pitch * TiltPitchFactor, -MaxTiltAngle, MaxTiltAngle);

	// Combine roll and pitch tilt
	FRotator CurrentRot = FollowCameraRef->GetRelativeRotation();
	FRotator TargetRot = FRotator(CurrentRot.Pitch + TargetPitchTilt, CurrentRot.Yaw, TargetRoll);

	// Smoothly interpolate rotation
	FRotator SmoothedRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, TiltInterpSpeed);

	FollowCameraRef->SetRelativeRotation(SmoothedRot);
}
void UCamComponent::UpdateHeadBob(float DeltaTime)
{
	if (!TraversalCharachter || !FollowCameraRef) return;

	const float Speed = TraversalCharachter->GetVelocity().Size2D();
	if (Speed < 10.f)
	{
		FollowCameraRef->SetRelativeLocation(FMath::VInterpTo(FollowCameraRef->GetRelativeLocation(), CameraDefaultLocation, DeltaTime, 10.f));
				
		return;
	}

	float BobSpeedFinal = BobSpeed;
	float BobAmountFinal = BobAmount;

	if (TraversalCharachter->MainComponents->StaminaComp->CanRun())
	{
		BobSpeedFinal *= RunBobMultiplier;
		BobAmountFinal *= RunBobMultiplier;
	}

	BobTimer += DeltaTime * BobSpeedFinal;

	FVector BobOffset = FVector(0.f, 0.f, FMath::Sin(BobTimer) * BobAmountFinal);
		
		

	FollowCameraRef->SetRelativeLocation(FMath::VInterpTo(FollowCameraRef->GetRelativeLocation(), CameraDefaultLocation + BobOffset, DeltaTime, 12.f));
		
		
		
}
void UCamComponent::UpdateDepthOfField(float DeltaTime)
{
	if (!FollowCameraRef) return;

	// Camera location and forward vector
	FVector Start = FollowCameraRef->GetComponentLocation();
	FVector End = Start + FollowCameraRef->GetForwardVector() * MaxFocusDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwningCharacter); // ignore player

	float TargetFocusDistance = MaxFocusDistance;
	bool bHitWall = false;

	// Capsule dimensions
	float CapsuleRadius = 10.0f; // adjust as needed
	float CapsuleHalfHeight = 10.0f;

	// Perform capsule trace
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		Params
	);

	// Debug draw the capsule
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(),
		Start + (End - Start) * 0.5f, // capsule center
		CapsuleHalfHeight,
		CapsuleRadius,
		FQuat::Identity,
		DebugColor,
		false,
		0.1f, // duration
		0,
		2.0f // thickness
	);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->ActorHasTag("Wall"))
		{
			bHitWall = true;
			TargetFocusDistance = FMath::Clamp(Hit.Distance, MinFocusDistance, MaxFocusDistance);
		}
	}

	// Smooth interpolation of focus distance
	CurrentFocusDistance = FMath::FInterpTo(CurrentFocusDistance, TargetFocusDistance, DeltaTime, FocusInterpSpeed);

	// Determine F-stop based on hit or not
	float BlurStrength = bHitWall ? 2.0f : 10.0f;
	// Strong blur if hit wall (low F-stop), weak blur otherwise (high F-stop)

	// Apply DOF
	FollowCameraRef->PostProcessSettings.DepthOfFieldFstop = BlurStrength;
	FollowCameraRef->PostProcessSettings.DepthOfFieldFocalDistance = CurrentFocusDistance;
	FollowCameraRef->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	FollowCameraRef->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
}

void UCamComponent::UpdateCameraShake()
{
	if (!PlayerController) return;
	if (!TraversalCharachter) return;

	// Get current speed in 2D
	float Speed = TraversalCharachter->GetVelocity().Size2D();

	// Determine which shake to use
	TSubclassOf<UCameraShakeBase> DesiredShake = nullptr;

	if (Speed < 10.f)
	{
		DesiredShake = IdleCameraShake;
	}
	else if (Speed <= 300.f)
	{
		DesiredShake = WalkCameraShake;
	}
	else
	{
		DesiredShake = RunCameraShake;
	}
	UCameraShakeBase* DefaultShake = DesiredShake->GetDefaultObject<UCameraShakeBase>();
	CurrentActiveShake = DefaultShake;
	PlayerController->ClientStartCameraShake(DesiredShake, 1.f);
	


}
#pragma endregion CameraFunctional



// Called when the game starts
void UCamComponent::BeginPlay()
{
	Super::BeginPlay();


	InitializeCharachter();
	InitializeCam();
	
}


// Called every frame
void UCamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ApplyLean(DeltaTime);
	UpdateFOV(DeltaTime);
	UpdateHeadBob(DeltaTime);
	UpdateTilt(DeltaTime);
	UpdateCameraPosition(DeltaTime);
	UpdateCameraRotation(DeltaTime);
	UpdateDepthOfField(DeltaTime);
	UpdateCameraShake();

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "VaultComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCrouchComponent::UCrouchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCrouchComponent::IntializeCrouch()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	Capsule = OwnerCharacter->GetCapsuleComponent();
	Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();

	if (Capsule)
	{
		StandingCapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	}

	if (Camera)
	{
		InitialCameraZ = Camera->GetRelativeLocation().Z;
	}

	TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	TargetCameraZ = InitialCameraZ;
}

void UCrouchComponent::StartCrouch()
{
	bWantsToCrouch = true;
	bIsCrouching = true;

	TargetCapsuleHalfHeight = CrouchedCapsuleHalfHeight;
	TargetCameraZ = InitialCameraZ - CameraCrouchOffset;

}

void UCrouchComponent::StopCrouch()
{
	if (!CanUnCrouch())
	{
		// Still blocked → stay crouched
		bWantsToCrouch = true;
		bIsCrouching = true;

		TargetCapsuleHalfHeight = CrouchedCapsuleHalfHeight;
		TargetCameraZ = InitialCameraZ - CameraCrouchOffset;
		return;
	}

	bWantsToCrouch = false;
	bIsCrouching = false;

	TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	TargetCameraZ = InitialCameraZ;
}


void UCrouchComponent::UpdateCrouch(float DeltaTime)
{
	UVaultComponent* VaultComp = OwnerCharacter->FindComponentByClass<UVaultComponent>();
	if (VaultComp && VaultComp->IsVaulting())
	{
		return;
	}

	if (!Capsule) return;


	    TargetSpeedCrouch = bIsCrouching ? CrouchSpeed : OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed,
		TargetSpeedCrouch,
		DeltaTime,
		SpeedInterp);
	


	if (!bIsCrouching && !CanUnCrouch())
	{
		StartCrouch();
	}
	


	// Smooth capsule height
	float CurrentHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	float NewHalfHeight = FMath::FInterpTo(CurrentHalfHeight, TargetCapsuleHalfHeight, DeltaTime, CrouchInterpSpeed);
		
	float HeightDelta = NewHalfHeight - CurrentHalfHeight;
	Capsule->SetCapsuleHalfHeight(NewHalfHeight, true);
		
	// Keep feet grounded
	OwnerCharacter->AddActorWorldOffset(
		FVector(0.f, 0.f, -HeightDelta)
	);

	// Smooth camera
	if (Camera)
	{
		FVector CamLoc = Camera->GetRelativeLocation();
		CamLoc.Z = FMath::FInterpTo(CamLoc.Z, TargetCameraZ, DeltaTime, CrouchInterpSpeed);
		Camera->SetRelativeLocation(CamLoc);
	
	}


	

}

bool UCrouchComponent::CanUnCrouch() const
{
	if (!Capsule || !OwnerCharacter)
		return false;

	const float Radius = Capsule->GetUnscaledCapsuleRadius();
	const float HalfHeightDifference = StandingCapsuleHalfHeight - CrouchedCapsuleHalfHeight;

	// Start at current capsule center
	FVector Start = OwnerCharacter->GetActorLocation() + FVector(0, 0, CrouchedCapsuleHalfHeight);
	FVector End = Start + FVector(0, 0, HalfHeightDifference);

	FCollisionShape Shape = FCollisionShape::MakeCapsule(Radius, StandingCapsuleHalfHeight);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->SweepTestByChannel(
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		Shape,
		Params
	);

#if WITH_EDITOR
	DrawDebugCapsule(
		GetWorld(),
		End,
		StandingCapsuleHalfHeight,
		Radius,
		FQuat::Identity,
		bHit ? FColor::Red : FColor::Green,
		false,
		0.1f
	);
#endif

	return !bHit;
}

// Called when the game starts
void UCrouchComponent::BeginPlay()
{
	Super::BeginPlay();

	IntializeCrouch();
	
}


// Called every frame
void UCrouchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCrouch(DeltaTime);
}


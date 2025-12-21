// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
USlideComponent::USlideComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USlideComponent::InitializeSlide()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		Capsule = OwnerCharacter->GetCapsuleComponent();
		Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();

		if (Camera)
		{
			InitialCameraZ = Camera->GetRelativeLocation().Z;
		}

		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		TargetCameraZ = InitialCameraZ;
	}
}

void USlideComponent::StartSlide()
{
	if (!CanSlide()) return;

	bIsSliding = true;
	bWantsToSlide = true;
	SlideTime = 0.f;

	// Set targets for smooth interpolation
	TargetCapsuleHalfHeight = SlideCapsuleHalfHeight;
	TargetCameraZ = InitialCameraZ - CameraSlideOffset;

	// Launch character forward
	FVector ForwardDir = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->GetCharacterMovement()->Velocity = ForwardDir * SlideSpeed;

	// Reduce friction for slide
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 0.f;
}

void USlideComponent::StopSlide()
{
	bIsSliding = false;
	bWantsToSlide = false;

	TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	TargetCameraZ = InitialCameraZ;

	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}


void USlideComponent::UpdateSlide(float DeltaTime)
{
	if (!Capsule || !Camera) return;



	// Smooth camera interpolation
	FVector CamLoc = Camera->GetRelativeLocation();
	CamLoc.Z = FMath::FInterpTo(CamLoc.Z, TargetCameraZ, DeltaTime, SlideInterpSpeed);
	Camera->SetRelativeLocation(CamLoc);

	// Slide timing
	if (bIsSliding)
	{
		// Smooth capsule interpolation
		float CurrentCapsule = Capsule->GetUnscaledCapsuleHalfHeight();
		float NewCapsule = FMath::FInterpTo(CurrentCapsule, TargetCapsuleHalfHeight, DeltaTime, SlideInterpSpeed);
		Capsule->SetCapsuleHalfHeight(NewCapsule);

		SlideTime += DeltaTime;
		if (SlideTime >= SlideDuration || !OwnerCharacter->GetCharacterMovement()->IsMovingOnGround())
		{
			StopSlide();
		}
	}
}

bool USlideComponent::CanSlide() const
{
	if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) return false;
	return OwnerCharacter->GetCharacterMovement()->IsMovingOnGround() && !bIsSliding && OwnerCharacter->GetVelocity().Size() > 300.f;
		
		
}

// Called when the game starts
void USlideComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlide();
	
}


// Called every frame
void USlideComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSlide(DeltaTime);
}


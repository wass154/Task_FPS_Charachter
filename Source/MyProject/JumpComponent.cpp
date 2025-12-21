// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJumpComponent::UJumpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UJumpComponent::InitializeJump()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	Movement = OwnerCharacter->GetCharacterMovement();
	Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();

	if (Camera)
	{
		InitialCameraPitch = Camera->GetRelativeRotation().Pitch;
		TargetCameraPitch = InitialCameraPitch;
	}
}

void UJumpComponent::StartJump()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "StartJumping Now ...");
	}

	if (!OwnerCharacter) return;
	if (!Movement->IsMovingOnGround()) return;
	if (!CanJump()) return;


	/* ========= PHYSICS JUMP =========
	   v = sqrt(2 * g * h)
	*/

	// Physics jump using impulse
	const float Gravity = FMath::Abs(GetWorld()->GetGravityZ()) * GravityScale;
	FVector JumpVelocity = FVector(0.f, 0.f, FMath::Sqrt(2.f * FMath::Abs(GetWorld()->GetGravityZ()) * JumpHeight));
	OwnerCharacter->LaunchCharacter(JumpVelocity, false, true);

	bIsJumping = true;
	bWantsToJump = true;

	// Camera kick
	TargetCameraPitch = InitialCameraPitch + JumpCameraPitch;


}

void UJumpComponent::StopJump()
{
	bWantsToJump = false;
}


void UJumpComponent::UpdateJump(float DeltaTime)
{
	if (!Movement) return;

	const bool bIsFalling = Movement->IsFalling();

	// Landing detection
	if (bWasFalling && !bIsFalling)
	{
		bIsJumping = false;
		ApplyLandingImpact();
	}

	// Apply camera interpolation
	ApplyCameraJump(DeltaTime);

	// Only reset jump state when grounded and camera finished moving
	if (Movement->IsMovingOnGround() && FMath::IsNearlyEqual(Camera->GetRelativeRotation().Pitch, InitialCameraPitch, 0.1f))
	{
		bIsJumping = false;
		TargetCameraPitch = InitialCameraPitch;
	}

	bWasFalling = bIsFalling;
}

void UJumpComponent::ApplyCameraJump(float DeltaTime)
{
	if (!Camera) return;

	FRotator CamRot = Camera->GetRelativeRotation();

	CamRot.Pitch = FMath::FInterpTo(CamRot.Pitch, TargetCameraPitch, DeltaTime, CameraInterpSpeed);

	Camera->SetRelativeRotation(CamRot);
}

bool UJumpComponent::CanJump() const
{
	return Movement && Movement->IsMovingOnGround();
}

void UJumpComponent::ApplyLandingImpact()
{
	TargetCameraPitch = InitialCameraPitch + LandingCameraPitch;
}

// Called when the game starts
void UJumpComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeJump();
	
}


// Called every frame
void UJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateJump(DeltaTime);
}


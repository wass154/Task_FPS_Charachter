// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementSystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h" // Added for GEngine

// Sets default values for this component's properties
UMovementSystems::UMovementSystems()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}

#pragma region Intiliaze
void UMovementSystems::Intialize()
{
	OwningCharacter = Cast<ACharacter>(GetOwner());

	if (!OwningCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("OwningCharacter is not attached to an ACharacter! %s"), *GetName()));
		return;
	}
	speed = 300.f;

}
float UMovementSystems::GetForwardValue() const
{
	return ForwardDirectionValue;
}
float UMovementSystems::GetRightValue() const
{
	return RightDirectionValue;
}
#pragma endregion Intiliaze


#pragma region MovementDirection
void UMovementSystems::MoveForward(float forward)
{

	if (forward != 0.0f && OwningCharacter)
	{
		ForwardDirectionValue = forward;
		OwningCharacter->AddMovementInput(FRotationMatrix(FRotator(0, OwningCharacter->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X), forward);

	}
}
void UMovementSystems::MoveRight(float right)
{
	if (right != 0.0f && OwningCharacter)
	{
		RightDirectionValue = right;
		OwningCharacter->AddMovementInput(FRotationMatrix(FRotator(0, OwningCharacter->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y), right);

	}
}
void UMovementSystems::UpdateControlRotation()
{
	if (!OwningCharacter || IsPlayerIdle())
		return;

	FRotator CurrentRotation = OwningCharacter->GetActorRotation();
	FRotator TargetRotation = OwningCharacter->Controller->GetControlRotation();

	FRotator TargetYawRotation(0.f, TargetRotation.Yaw, 0.f);
	FRotator CurrentYawRotation(0.f, CurrentRotation.Yaw, 0.f);

	FRotator SmoothYawRotation = FMath::RInterpTo(CurrentYawRotation, TargetYawRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 4.f);

	OwningCharacter->SetActorRotation(SmoothYawRotation);
}
void UMovementSystems::InterpolateMovementSpeed(float TargetSpeed)
{
	if (OwningCharacter)
	{
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(OwningCharacter->GetCharacterMovement()->MaxWalkSpeed, TargetSpeed, UGameplayStatics::GetWorldDeltaSeconds(this), 10.f);
	}
}
void UMovementSystems::UpdateMovementState()
{
	if (IsPlayerIdle())
	{
		TraversalState = EMovement::Idle;

	}
	else if ( OwningCharacter->GetCharacterMovement()->Velocity.Length()< speed)
	{
		TraversalState = EMovement::Walking;
	}
	else
	{
		TraversalState = EMovement::Running;
	}
}
bool UMovementSystems::IsPlayerIdle() const
{
	return OwningCharacter->GetCharacterMovement()->Velocity.Size() < KINDA_SMALL_NUMBER;

}
#pragma endregion MovementDirection






// Called when the game starts
void UMovementSystems::BeginPlay()
{
	Super::BeginPlay();

	Intialize();
	
}


// Called every frame
void UMovementSystems::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMovementState();
}


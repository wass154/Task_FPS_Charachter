// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


void UStaminaComponent::StaminaTick(float DeltaTime)
{

	if (bIsRunning && CanRun())
	{
		ConsumeStamina();
	}
	else if(!bIsRunning && CurrentStamina<MaxStamina)
	{
		RestoreStamina();
	}


}

void UStaminaComponent::StaminaIntializer()
{
	MaxStamina = 100.f;
	StaminaDrainRate = 20.f;
	StaminaRegenRate = 10.f;
	RunSpeed = 600.f;
	WalkSpeed = 300.f;
	bIsRunning = false;
	bCanRegen = false;
	owner = Cast<ACharacter>(GetOwner());

	if (owner)
	{
		MoveComp = owner->GetCharacterMovement();
	}
}

void UStaminaComponent::StartRunning()
{

	if (CanRun())
	{
		bIsRunning = true;
		if (owner && owner->GetCharacterMovement())
		{
			owner->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
	}

}

void UStaminaComponent::StopRunning()
{

		bIsRunning = false;
		if (owner && owner->GetCharacterMovement())
		{
			owner->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			StartStaminaRegen();
		}

}

//return is Run or not 
bool UStaminaComponent::CanRun() const
{
	return CurrentStamina > 0;
}

void UStaminaComponent::ConsumeStamina()
{
	if (bIsRunning)
	{
		CurrentStamina = FMath::Max(0.0f, CurrentStamina - (StaminaDrainRate * GetWorld()->GetDeltaSeconds())); 
	}
	if (CurrentStamina <= 0)
	{
		StopRunning();
		bCanRegen = false;
		GetWorld()->GetTimerManager().SetTimer(
			regenDelayTimer,
			this,
			&UStaminaComponent::StartStaminaRegen,
			3.f,  //delay

			false
		);
	}
}


void UStaminaComponent::RestoreStamina()
{
	if (!bIsRunning && bCanRegen)
	{
		bCanRegen = true;
		CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + (StaminaRegenRate * GetWorld()->GetDeltaSeconds()));

   }
}

void UStaminaComponent::StartStaminaRegen()
{
	bCanRegen = true;
}

float UStaminaComponent::GetStaminaPercentage() const
{
	return CurrentStamina > 0 ? (CurrentStamina / MaxStamina) * 100.f : 0.f;
}

// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	StaminaIntializer();

	CurrentStamina = MaxStamina;
	owner->GetCharacterMovement()->MaxWalkSpeed = 300.f;


}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StaminaTick(DeltaTime);
	// ...
}


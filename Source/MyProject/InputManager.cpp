// Fill out your copyright notice in the Description page of Project Settings.


#include "InputManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


// Sets default values for this component's properties
UInputManager::UInputManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInputManager::IntiliazeInput()
{

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	
	{
		if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				InputSubsystem = Subsystem;
			}

			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				EnhancedInputComponent = EIC;
			}
		}
	}
	

}

void UInputManager::AddMappingContext(UInputMappingContext* MappingContext, int32 Priority)
{
	if (InputSubsystem && MappingContext)
	{
		InputSubsystem->AddMappingContext(MappingContext, Priority);
	}
}

void UInputManager::RemoveMappingContext(UInputMappingContext* MappingContext)
{
	if (InputSubsystem && MappingContext)
	{
		InputSubsystem->RemoveMappingContext(MappingContext);
	}
}

void UInputManager::BindAction(UInputAction* Action, ETriggerEvent TriggerEvent, UObject* Object, FName FunctionName)
{
	if (!EnhancedInputComponent || !Action) return;

	EnhancedInputComponent->BindAction(Action, TriggerEvent, Object, FunctionName);
}

void UInputManager::BindActionByName(FName ActionName, ETriggerEvent TriggerEvent, UObject* Object, FName FunctionName)
{
	if (!EnhancedInputComponent) return;

	// Find the action in the array by name
	for (UInputAction* Action : InputActions)
	{
		if (!Action) continue;

		if (Action->GetFName() == ActionName)
		{
			EnhancedInputComponent->BindAction(Action, TriggerEvent, Object, FunctionName);
			break;
		}
	}
}


// Called when the game starts
void UInputManager::BeginPlay()
{
	Super::BeginPlay();

	IntiliazeInput();
	AddMappingContext(DefaultMappingContext);
}


// Called every frame
void UInputManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


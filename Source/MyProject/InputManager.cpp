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

#pragma region Intilazation
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
#pragma endregion Intilazation



#pragma region InputSetups
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

void UInputManager::InitializeInputComponent(UInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = CastChecked< UEnhancedInputComponent>(PlayerInputComponent);


	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager: PlayerInputComponent is NOT EnhancedInputComponent"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "InputManager: PlayerInputComponent is NOT EnhancedInputComponent Now ...");

		return;
	}

	

}

#pragma endregion InputSetups



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


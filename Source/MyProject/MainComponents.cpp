
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainComponents.h"

// Sets default values for this component's properties
UMainComponents::UMainComponents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	MovementSystem = CreateDefaultSubobject<UMovementSystems>(TEXT("MovementSystem"));

	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));

	CamComp = CreateDefaultSubobject<UCamComponent>(TEXT("CamComp"));


	CrouchComp = CreateDefaultSubobject<UCrouchComponent>(TEXT("CrouchComp"));

	SlideComp = CreateDefaultSubobject<USlideComponent>(TEXT("SlideComp"));

	VaultComp= CreateDefaultSubobject<UVaultComponent>(TEXT("VaultComp"));

	LedgeComp= CreateDefaultSubobject<ULedgeComponent>(TEXT("LedgeComp"));

	JumpComp = CreateDefaultSubobject<UJumpComponent>(TEXT("JumpComp"));

	
}



// Called when the game starts
void UMainComponents::BeginPlay()
{
	Super::BeginPlay();

	if (!MovementSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovementSystem was null at runtime. Attempting to find or recover."));
	}

	if (!StaminaComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("StaminaComp was null at runtime. Attempting to find or recover."));
	}

	if (!CamComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CamComp was null at runtime. Attempting to find or recover."));
	}

	if (!CrouchComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CrouchComp was null at runtime. Attempting to find or recover."));
	}

	if (!SlideComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("SlideComp was null at runtime. Attempting to find or recover."));
	}

	if (!VaultComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("VaultComp was null at runtime. Attempting to find or recover."));
	}

	if (!LedgeComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("LedgeComp was null at runtime. Attempting to find or recover."));
	}

	if (!JumpComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("JumpComp was null at runtime. Attempting to find or recover."));
	}
}


// Called every frame
void UMainComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


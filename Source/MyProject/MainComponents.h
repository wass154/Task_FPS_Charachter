// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementSystems.h"
#include "StaminaComponent.h"
#include "CamComponent.h"
#include "InputManager.h"
#include "CrouchComponent.h"
#include "SlideComponent.h"
#include "VaultComponent.h"
#include "LedgeComponent.h"
#include "JumpComponent.h"
#include "MainComponents.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMainComponents : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMainComponents();


   //Components Used With Main Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UMovementSystems* MovementSystem;

	//Components Used With Main Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaminaComponent* StaminaComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCamComponent* CamComp;
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCrouchComponent* CrouchComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USlideComponent* SlideComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UVaultComponent* VaultComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	ULedgeComponent* LedgeComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UJumpComponent* JumpComp;
	
	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "EMovementState.h"
#include "StaminaComponent.generated.h"


class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	/*Movement State*/


	/*Stamina Timer*/
	FTimerHandle regenDelayTimer;

	/*Stamina Variables*/

	UPROPERTY(VisibleAnywhere,Category="Stamina|Bools")
	bool bIsRunning;
	UPROPERTY(VisibleAnywhere, Category = "Stamina|Bools")
	bool bCanRegen;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "600.0"))
	float RunSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "300.0"))
	float WalkSpeed;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "100.0"))
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "100.0"))
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "100.0"))
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Vars", meta = (ClampMin = "0.0"), meta = (ClampMax = "100.0"))
	float StaminaRegenRate;


	/*Functions*/

	UFUNCTION(BlueprintCallable,Category="Stamina")
	void StaminaTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StaminaIntializer();



	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StopRunning();

	UFUNCTION()
	bool CanRun() const;

	UFUNCTION()
	void ConsumeStamina();

	UFUNCTION()
	void RestoreStamina();

	UFUNCTION()
	void StartStaminaRegen();

	UFUNCTION(BlueprintCallable)
	float GetStaminaPercentage() const;


	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaNormalized() const;

	

private:
	ACharacter* owner = nullptr;
	UCharacterMovementComponent* MoveComp = nullptr;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

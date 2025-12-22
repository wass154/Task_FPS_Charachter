// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMovementState.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "MovementSystems.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMovementSystems : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementSystems();



	/* Current traversal state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal")
	EMovement TraversalState;

	UPROPERTY(VisibleAnywhere, Category = Charachter)
	ACharacter* OwningCharacter = nullptr; // Cached reference From PlayerCharachter


	/* Intialize Functions  */
	UFUNCTION(BlueprintCallable)
	void Intialize();



	/* Speed Variables */
	UPROPERTY(EditAnywhere,Category="Movement|Speeds")
	float MaxSpeed;


	UPROPERTY(VisibleAnywhere, Category = "Movement|Speeds")
	float speed;


	UPROPERTY(VisibleAnywhere, Category = "Movement|Speeds")
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Directions")
	float ForwardDirectionValue;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Directions")
	float RightDirectionValue;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Input")
	float GetForwardValue() const;

	/** Get current right input value (can be called from other scripts) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Input")
	float GetRightValue() const;


	UPROPERTY(EditAnywhere, Category = "Movement|LinearVelocity")
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = "Movement|Quaternion")
	FRotator Rot;


	
	/* MovementDirection */
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);

	UFUNCTION()
	void UpdateControlRotation();

	UFUNCTION()
	void InterpolateMovementSpeed(float TargetSpeed);

	UFUNCTION()
	void UpdateMovementState();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerIdle()const;

	

private:


	







protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

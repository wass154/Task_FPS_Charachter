// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrouchComponent.generated.h"



class ACharacter;
class UCapsuleComponent;
class UCameraComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCrouchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrouchComponent();


	// ===== Owner =====
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UCapsuleComponent* Capsule;

	UPROPERTY()
	UCameraComponent* Camera;


	UFUNCTION(BlueprintCallable,Category="Crouch|Intialize")
	void IntializeCrouch();



	// ===== API Crouch =====
	UFUNCTION(BlueprintCallable, Category = "Crouch|Core")
	void StartCrouch();
	UFUNCTION(BlueprintCallable, Category = "Crouch|Core")
	void StopCrouch();


	// ===== Settings =====
	UPROPERTY(EditAnywhere, Category = "Crouch")
	float StandingCapsuleHalfHeight = 88.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchedCapsuleHalfHeight = 44.f;

	// Movement
	UPROPERTY(EditAnywhere, Category = "Crouch|Movement")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Crouch|Movement")
	float CrouchSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Crouch|Movement")
	float TargetSpeedCrouch = 300.f;
	 

	UPROPERTY(EditAnywhere, Category = "Crouch|Movement")
	float SpeedInterp = 8.f;



	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CameraCrouchOffset = 32.f;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchInterpSpeed = 12.f;

	// ===== States =====
	UPROPERTY(VisibleAnywhere, Category = "Crouch|Bools")
	bool bIsCrouching = false;

	UPROPERTY(VisibleAnywhere, Category = "Crouch|Bools")
	bool bWantsToCrouch = false;


	UPROPERTY(VisibleAnywhere, Category = "Crouch|Floats")
	float TargetCapsuleHalfHeight;


	UPROPERTY(VisibleAnywhere, Category = "Crouch|Floats")
	float TargetCameraZ;


	UPROPERTY(VisibleAnywhere, Category = "Crouch|Floats")
	float InitialCameraZ;

	// ===== Internal =====
	void UpdateCrouch(float DeltaTime);
	bool CanUnCrouch() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

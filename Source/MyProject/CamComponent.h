// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/UObjectGlobals.h"
#include "CamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCamComponent();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoomRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCameraRef;

	class AMyTraversalCharachter* TraversalCharachter;

	UPROPERTY()
	class ACharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Charachter")
	APlayerController* PlayerController; 
	UPROPERTY(VisibleAnywhere, Category = "Charachter")
	APlayerCameraManager* CameraManager; 
	
	UFUNCTION()
	void InitializeCharachter();


	//IntilizeCamera Functions

	UFUNCTION()
	void InitializeCam();

	UFUNCTION()
	void UpdateCameraPosition(float DeltaTime);

	UFUNCTION()
	void UpdateCameraRotation(float DeltaTime);


	UFUNCTION()
	void LookUp(float Value);
	UFUNCTION()
	void Turn(float Value);

	UFUNCTION()
	void Lean(float Value);


	UFUNCTION()
	void ApplyLean(float DeltaTime);


	/* ===================== INTERNAL FUNCTIONS ===================== */
	UFUNCTION()
	float GetTargetFOV() const;

	UFUNCTION()
	void UpdateFOV(float DeltaTime);

	UFUNCTION()
	void UpdateTilt(float DeltaTime);

	UFUNCTION()
	void UpdateHeadBob(float DeltaTime);

	UFUNCTION()
	void UpdateDepthOfField(float DeltaTime);

	UFUNCTION()
	void UpdateCameraShake();


	/* ===================== CAMERA SHAKES ===================== */
	UPROPERTY(EditAnywhere, Category = "Camera|Shakes")
	TSubclassOf<class UCameraShakeBase> IdleCameraShake;

	UPROPERTY(EditAnywhere, Category = "Camera|Shakes")
	TSubclassOf<class UCameraShakeBase> WalkCameraShake;

	UPROPERTY(EditAnywhere, Category = "Camera|Shakes")
	TSubclassOf<class UCameraShakeBase> RunCameraShake;


	UPROPERTY(VisibleAnywhere, Category = "Camera|Shakes")
	UCameraShakeBase* CurrentActiveShake = nullptr;



	/* ===================== SETTINGS ===================== */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float LookUpRate = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float TurnRate = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float MinPitch = -80.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float MaxPitch = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Smoothing")
	float SmoothSpeed = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FPS")
	float MaxLeanAngle = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Smoothing")
	float LeanSmoothSpeed = 6.f;


	UPROPERTY(EditAnywhere, Category = "Camera|Tilt")
	float TiltYawFactor = 0.1f; // How much roll per degree of yaw

	UPROPERTY(EditAnywhere, Category = "Camera|Tilt")
	float TiltPitchFactor = 0.05f; // Optional pitch tilt factor



	/* ===================== DEPTH OF FIELD ===================== */
	UPROPERTY(EditAnywhere, Category = "Camera|DoF")
	float MaxFocusDistance = 1000.f; // Max distance for focus

	UPROPERTY(EditAnywhere, Category = "Camera|DoF")
	float MinFocusDistance = 200.f; // Min distance for near blur

	UPROPERTY(EditAnywhere, Category = "Camera|DoF")
	float FocusInterpSpeed = 8.f; // Speed to interpolate focus distance

	UPROPERTY(VisibleAnywhere, Category = "Camera|DoF")
	float CurrentFocusDistance = 1000.f;


	/* ===================== CAMERA DYNAMICS ===================== */

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float IdleFOV = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float WalkFOV = 92.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float RunFOV = 98.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float CrouchFOV = 88.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float SlideFOV = 104.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float VaultFOV = 96.f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float FOVInterpSpeed = 8.f;


	/* ===================== TILT ===================== */

	UPROPERTY(EditAnywhere, Category = "Camera|Tilt")
	float MaxTiltAngle = 6.f;

	UPROPERTY(EditAnywhere, Category = "Camera|Tilt")
	float TiltInterpSpeed = 8.f;


	/* ===================== HEAD BOB ===================== */

	UPROPERTY(EditAnywhere, Category = "Camera|Bob")
	float BobAmount = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Camera|Bob")
	float BobSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Camera|Bob")
	float RunBobMultiplier = 1.6f;


	/* ===================== INTERNAL ===================== */

	UPROPERTY(VisibleAnywhere, Category = "Camera|FOV")
	float TargetFOV;
	UPROPERTY(VisibleAnywhere, Category = "Camera|FOV")
	float CurrentFOV;

	UPROPERTY(VisibleAnywhere, Category = "Camera|Tilt")
	float TargetTilt;
	UPROPERTY(VisibleAnywhere, Category = "Camera|Tilt")
	float CurrentTilt;

	UPROPERTY(VisibleAnywhere, Category = "Camera|Tilt")
	float BobTimer = 0.f;
	FVector CameraDefaultLocation;

private:
	float TargetLean = 0.f;
	float CurrentLean = 0.f;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

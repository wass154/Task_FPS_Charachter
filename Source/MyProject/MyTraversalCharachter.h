// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainComponents.h"
#include "ECameraType.h"
#include "InputManager.h"
#include "StructureProprities.h"
#include "MyTraversalCharachter.generated.h"

UCLASS()
class MYPROJECT_API AMyTraversalCharachter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyTraversalCharachter();


	/////////////////// SkeletalMeshComponentComponents////////////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ComponentsBodyMesh")
	class	USkeletalMeshComponent* BodyMesh; // Additional skeletal mesh for body
	/////////////////// SkeletalMeshComponentComponents////////////////////////////////



	///////////////////  CameraAndSpringArmComponents ////////////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cam")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cam")
	class UCameraComponent* FollowCamera;
	///////////////////  CameraAndSpringArmComponents ////////////////////////////////


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMainComponents* MainComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	UInputManager* InputManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComponentsCameras")
	FStructureProprities Components;

	
	/* ================= CAMERA MODE ================= */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cam")
	ECameraMode DefaultCameraMode = ECameraMode::FPS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cam")
	ECameraMode CurrentCameraMode;

	/* ================= API ================= */

	UFUNCTION(BlueprintCallable, Category = "Cam")
	void SetCameraMode(ECameraMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "Cam")
	void ToggleCameraMode();







	//two pointers function get camera and spring arm
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetCameraFollow() const { return FollowCamera; }


private:
	void ApplyCameraMode(ECameraMode Mode);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

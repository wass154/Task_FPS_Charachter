// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpComponent.generated.h"


class ACharacter;
class UCharacterMovementComponent;
class UCameraComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UJumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJumpComponent();

	// ===== Owner =====
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* Movement;

	UPROPERTY()
	UCameraComponent* Camera;

	UFUNCTION(BlueprintCallable, Category = "Jump|Initialize")
	void InitializeJump();

	// ===== API Jump =====
	UFUNCTION(BlueprintCallable, Category = "Jump|Core")
	void StartJump();

	UFUNCTION(BlueprintCallable, Category = "Jump|Core")
	void StopJump();

	// ===== Settings =====

	/** Jump height in cm (physics based) */
	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpHeight = 200.f;

	/** Extra gravity control */
	UPROPERTY(EditAnywhere, Category = "Jump")
	float GravityScale = 1.0f;

	// ===== Camera =====

	UPROPERTY(EditAnywhere, Category = "Jump|Camera")
	float JumpCameraPitch = -5.f;

	UPROPERTY(EditAnywhere, Category = "Jump|Camera")
	float CameraInterpSpeed = 10.f;

	// ===== States =====

	UPROPERTY(VisibleAnywhere, Category = "Jump|Bools")
	bool bIsJumping = false;

	UPROPERTY(VisibleAnywhere, Category = "Jump|Bools")
	bool bWantsToJump = false;

	// ===== Floats =====

	UPROPERTY(VisibleAnywhere, Category = "Jump|Floats")
	float TargetCameraPitch = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Jump|Floats")
	float InitialCameraPitch = 0.f;

	// ===== Internal =====
	void UpdateJump(float DeltaTime);
	void ApplyCameraJump(float DeltaTime);
	bool CanJump() const;

	// ===== Camera (Landing) =====
	UPROPERTY(EditAnywhere, Category = "Jump|Camera")
	float LandingCameraPitch = 4.f;

	UPROPERTY(VisibleAnywhere, Category = "Jump|Bools")
	bool bWasFalling = false;

	// ===== Internal =====
	void ApplyLandingImpact();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

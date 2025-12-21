// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlideComponent.generated.h"


class ACharacter;
class UCapsuleComponent;
class UCameraComponent;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API USlideComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlideComponent();

	// ===== Owner =====
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UCapsuleComponent* Capsule;

	UPROPERTY()
	UCameraComponent* Camera;


	UFUNCTION(BlueprintCallable, Category = "Slide|Initialize")
	void InitializeSlide();

	// ===== API Slide =====
	UFUNCTION(BlueprintCallable, Category = "Slide|Core")
	void StartSlide();

	UFUNCTION(BlueprintCallable, Category = "Slide|Core")
	void StopSlide();

	// ===== Settings =====
	UPROPERTY(EditAnywhere, Category = "Slide")
	float StandingCapsuleHalfHeight = 88.f;

	UPROPERTY(EditAnywhere, Category = "Slide")
	float SlideCapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, Category = "Slide")
	float CameraSlideOffset = 32.f;

	UPROPERTY(EditAnywhere, Category = "Slide")
	float SlideInterpSpeed = 12.f;

	UPROPERTY(EditAnywhere, Category = "Slide")
	float SlideDuration = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Slide")
	float SlideSpeed = 1200.f;

	// ===== States =====
	UPROPERTY(VisibleAnywhere, Category = "Slide|Bools")
	bool bIsSliding = false;

	UPROPERTY(VisibleAnywhere, Category = "Slide|Bools")
	bool bWantsToSlide = false;

	UPROPERTY(VisibleAnywhere, Category = "Slide|Floats")
	float TargetCapsuleHalfHeight;

	UPROPERTY(VisibleAnywhere, Category = "Slide|Floats")
	float TargetCameraZ;

	UPROPERTY(VisibleAnywhere, Category = "Slide|Floats")
	float InitialCameraZ;

private:
	float SlideTime = 0.f;

	// ===== Internal =====
	void UpdateSlide(float DeltaTime);
	bool CanSlide() const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaWidget.generated.h"


class UProgressBar;
class UStaminaComponent;
class AMyTraversalCharachter;
class UWidgetAnimation;


UCLASS()
class MYPROJECT_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	

	/* ===== Widgets ===== */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	/* ===== Animations ===== */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIN;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOut;

	/* ===== Cached References ===== */
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	AMyTraversalCharachter* TraversalCharachter;

	UPROPERTY()
	UStaminaComponent* StaminaComponent;

	/* ===== Internal ===== */
	float SmoothedPercent = 0.f;
	bool bIsVisible = false;

	/* ===== Logic ===== */
	void InitializeCharacter();
	void UpdateStamina(float DeltaTime);
	void UpdateVisibility();


};

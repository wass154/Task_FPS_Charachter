// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API ULedgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULedgeComponent();


	/*Varibles*/


	UPROPERTY(EditAnywhere, Category = "Ledge")
	float LedgeDistance;

	UPROPERTY(EditAnywhere, Category = "Ledge")
	float maxLedge;

	UPROPERTY(VisibleAnywhere, Category = "Ledge")
	FVector LedgePostion;

	UPROPERTY(VisibleAnywhere, Category = "Ledge")
	FVector CurrentPostion;

	UPROPERTY(VisibleAnywhere, Category = "Ledge")
	FRotator CurrentRotation;

	UPROPERTY(VisibleAnywhere, Category = "Ledge")
	bool bIsLedging;

	UPROPERTY(VisibleAnywhere, Category = "Ledge")
	bool bIsdropped;


	UFUNCTION(BlueprintCallable)
	void IntializeLedge();

	UFUNCTION()
	void TryLedge();

	UFUNCTION()
	void StartLedge();
	
	UFUNCTION()
	void StopLedge();

	UFUNCTION()
	bool CanLedge();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

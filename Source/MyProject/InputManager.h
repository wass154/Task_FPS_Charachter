// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputManager.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionDelegate, const FInputActionValue&, Value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInputManager : public UActorComponent
{
	GENERATED_BODY()



public:	
	// Sets default values for this component's properties
	UInputManager();


	// References to mapping context and input actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultMappingContext;


	// Input actions stored in a list for easy iteration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Actions")
	TArray<UInputAction*> InputActions;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Actions")
	UInputAction* TryTestVault;





	UFUNCTION()
	void IntiliazeInput();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddMappingContext(UInputMappingContext* MappingContext, int32 Priority = 0);


	UFUNCTION(BlueprintCallable, Category = "Input")
	void RemoveMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void BindAction(UInputAction* Action, ETriggerEvent TriggerEvent, UObject* Object, FName FunctionName);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void BindActionByName(FName ActionName, ETriggerEvent TriggerEvent, UObject* Object, FName FunctionName);



private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "UObject/UObjectGlobals.h"
#include "GameFramework/SpringArmComponent.h"
#include"StructureProprities.generated.h"

USTRUCT(BlueprintType)
struct FStructureProprities
{
    GENERATED_BODY()

public:
  /*Raw pointers to components*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traversal|Spring References")
    USpringArmComponent* SpringArm = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traversal|Camera References")
    UCameraComponent* Camera = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traversal|Charachter References")
    UCharacterMovementComponent* CharacterMovement = nullptr;

public:

    /* This function sets the struct references from given parameters*/
    void SetComponents(USpringArmComponent* InSpringArm, UCameraComponent* InCamera, UCharacterMovementComponent* InCharacterMovement)
    {
        SpringArm = InSpringArm;
        Camera = InCamera;
        CharacterMovement = InCharacterMovement;
    }
};
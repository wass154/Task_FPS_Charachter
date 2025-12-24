// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TraversalBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UTraversalBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    UFUNCTION(BlueprintPure, Category = "Traversal")
    static float GetCharacterStaminaNormalized(ACharacter* Character);

    UFUNCTION(BlueprintPure, Category = "Traversal")
    static bool IsCharacterJumping(ACharacter* Character);

    UFUNCTION(BlueprintPure, Category = "Traversal")
    static bool IsCharacterVaulting(ACharacter* Character);
	
};

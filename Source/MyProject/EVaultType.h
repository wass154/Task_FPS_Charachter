// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EVault : uint8
{
    Vaulting    UMETA(DisplayName = "Vaulting"),
    Mantling    UMETA(DisplayName = "Mantling"),
    Ledging     UMETA(DisplayName = "Ledging"),
    None        UMETA(DisplayName = "None")
};
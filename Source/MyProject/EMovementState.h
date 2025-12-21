#pragma once

#include "CoreMinimal.h"
#include "EMovementState.generated.h"



UENUM(BlueprintType)
enum class EMovement : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Running     UMETA(DisplayName = "Running"),
    Jumping     UMETA(DisplayName = "Jumping"),
    Crouching   UMETA(DisplayName = "Crouching"),
    Sliding     UMETA(DisplayName = "Sliding"),
    Vaulting    UMETA(DisplayName = "Vaulting")
};
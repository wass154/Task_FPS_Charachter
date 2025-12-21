#pragma once

#include "CoreMinimal.h"
#include "ECameraType.generated.h"

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	FPS UMETA(DisplayName = "First Person"),
	TPS UMETA(DisplayName = "Third Person")
};

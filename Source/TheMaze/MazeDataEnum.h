#pragma once

#include "CoreMinimal.h"
#include "MazeDataEnum.generated.h"


UENUM(BlueprintType)
enum class EKeyDoorTier : uint8 {
	KeyDoor_Common		UMETA(DisplayName = "Common"),
	KeyDoor_Uncommon	UMETA(DisplayName = "Uncommon"),
	KeyDoor_Rare		UMETA(DisplayName = "Rare"),
};

UENUM(BlueprintType)
enum class EDirection : uint8 {
	NORTH = 0	UMETA(DisplayName = "North"),
	EAST		UMETA(DisplayName = "East"),
	SOUTH		UMETA(DisplayName = "South"),
	WEST		UMETA(DisplayName = "West"),
	UNDEF		UMETA(DisplayName = "Undefined"),
};
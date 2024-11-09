#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MazeData.generated.h"


UENUM(BlueprintType)
enum class EKeyDoorTier : uint8 {
	KeyDoor_Common		UMETA(DisplayName = "Common"),
	KeyDoor_Uncommon	UMETA(DisplayName = "Uncommon"),
	KeyDoor_Rare		UMETA(DisplayName = "Rare"),
};


USTRUCT(BlueprintType)
struct FTierProperty : public FTableRowBase {
	GENERATED_BODY()

	// The enum tier type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiers")
	EKeyDoorTier Tier;

	// Colour of the tier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiers")
	FColor  Colour;

	// Probability of the tier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiers")
	float Proba;

};
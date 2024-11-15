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

USTRUCT(BlueprintType)
struct FNode {
	GENERATED_BODY()

	// Is the Node a DeadEnd path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	bool isDeadEnd;

	// Direction the Node is pointing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FVector2D LinkDirection;

	FNode(FVector2D Direction)
	{
		isDeadEnd = false;
		LinkDirection = Direction;
	}

	FNode()
	{
		isDeadEnd = false;
		LinkDirection = FVector2D(0, 0);
	}
};
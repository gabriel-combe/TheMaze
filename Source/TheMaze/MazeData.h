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

	// Position of the Node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FVector2D Position;

	// Direction the Node is pointing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FVector2D LinkDirection;

	// Number of node pointing to this one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int LinkNumberFromOthers;

	FNode(FVector2D Pos, FVector2D Direction, bool DeadEnd)
	{
		isDeadEnd = DeadEnd;
		Position = Pos;
		LinkDirection = Direction;
		LinkNumberFromOthers = 0;
	}

	FNode(FVector2D Pos, FVector2D Direction)
	{
		isDeadEnd = false;
		Position = Pos;
		LinkDirection = Direction;
		LinkNumberFromOthers = 0;
	}

	FNode()
	{
		isDeadEnd = false;
		Position = FVector2D(0, 0);
		LinkDirection = FVector2D(0, 0);
		LinkNumberFromOthers = 0;
	}

	void SetDirection(FVector2D NewDirection)
	{
		LinkDirection = NewDirection;
	}

	void SetLinkNbOthers(int count)
	{
		LinkNumberFromOthers = count;
	}

	void AddLinkNbOthers()
	{
		LinkNumberFromOthers++;
	}

	void RemoveLinkNbOthers()
	{
		LinkNumberFromOthers--;
	}
};
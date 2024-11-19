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

UENUM(BlueprintType)
enum class EDirection : uint8 {
	NORTH = 0	UMETA(DisplayName = "North"),
	EAST		UMETA(DisplayName = "East"),
	SOUTH		UMETA(DisplayName = "South"),
	WEST		UMETA(DisplayName = "West"),
	UNDEF		UMETA(DisplayName = "Undefined"),
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

	// List of walls for this node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	TArray<int> WallArray;

	FNode(FVector2D Pos, FVector2D Direction, bool DeadEnd)
	{
		isDeadEnd = DeadEnd;
		Position = Pos;
		LinkDirection = Direction;
		LinkNumberFromOthers = 0;
		WallArray.Init(-1, 4);
	}

	FNode(FVector2D Pos, FVector2D Direction)
	{
		isDeadEnd = false;
		Position = Pos;
		LinkDirection = Direction;
		LinkNumberFromOthers = 0;
		WallArray.Init(-1, 4);
	}

	FNode()
	{
		isDeadEnd = false;
		Position = FVector2D(0, 0);
		LinkDirection = FVector2D(0, 0);
		LinkNumberFromOthers = 0;
		WallArray.Init(-1, 4);
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

		isDeadEnd = false;
	}

	void RemoveLinkNbOthers()
	{
		if (LinkNumberFromOthers == 0) return;

		LinkNumberFromOthers--;

		if (LinkNumberFromOthers == 0)
			isDeadEnd = true;
	}
};
#pragma once

#include "CoreMinimal.h"
#include "DoorObject.h"
#include "KeyItem.h"
#include "Engine/DataTable.h"
#include "MazeDataStruct.generated.h"

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

	// Door associated to this node if it is a dead end
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	TObjectPtr<ADoorObject> Door = nullptr;

	// Item associated to this node if it is a dead end
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	TObjectPtr<AActor> Item = nullptr;

	// Check if it has item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	bool HasItem = false;

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

	bool operator==(const FNode& other) const{
		return Position == other.Position;
	}

	void SetDirection(FVector2D NewDirection)
	{
		LinkDirection = NewDirection;

		CheckIsDeadEnd();
	}

	void SetLinkNbOthers(int count)
	{
		LinkNumberFromOthers = count;

		CheckIsDeadEnd();
	}

	void AddLinkNbOthers()
	{
		LinkNumberFromOthers++;

		CheckIsDeadEnd();
	}

	void RemoveLinkNbOthers()
	{
		if (LinkNumberFromOthers == 0) return;

		LinkNumberFromOthers--;

		CheckIsDeadEnd();
	}

	void CheckIsDeadEnd()
	{
		isDeadEnd = (LinkNumberFromOthers == 0 && LinkDirection != FVector2D::ZeroVector) || (LinkNumberFromOthers == 1 && LinkDirection == FVector2D::ZeroVector);
	}

	void UpdateTransformItem(FVector2D dir, int CellSize)
	{
		int RotFactor = 0;

		if (dir.X == -1)
			RotFactor = 1;
		else if (dir.Y == -1)
			RotFactor = 2;
		else if (dir.X == 1)
			RotFactor = 3;

		Item->SetActorRelativeTransform(FTransform(FRotator(0, 90 * RotFactor, 0), FVector(Position.X * CellSize, Position.Y * CellSize, 0), FVector::OneVector));

		if (Door)
			Door->SetActorRelativeTransform(FTransform(FRotator(0, 90 * RotFactor, 0), FVector(dir.X * (CellSize*0.5f) + Position.X * CellSize, dir.Y * (CellSize * 0.5f) + Position.Y * CellSize, 0), FVector::OneVector));
	}
};
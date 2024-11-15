// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeData.h"
#include "MazeGameInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class THEMAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()

private:
	// Position of the origin point
	FIntVector2 Origin;

	// Position of the next origin point
	FIntVector2 NextOrigin;

	// Array of possible direction
	TArray<FIntVector2> PossibleDirection = TArray<FIntVector2>({
		FIntVector2(-1, 0),
		FIntVector2(0, -1),
		FIntVector2(1, 0),
		FIntVector2(0, 1)
	});

	/** Maze game instance */
	TObjectPtr<UMazeGameInstance> MazeGI;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Instanced static mesh component for the floors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Mesh")
	TObjectPtr<UInstancedStaticMeshComponent> ISMFloorComponent;

	// Instanced static mesh component for the walls
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Mesh")
	TObjectPtr<UInstancedStaticMeshComponent> ISMWallComponent;

	// Width of the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int Width;

	// Height of the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int Height;

	// Size of each cell of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze|Settings", meta = (ClampMin = "20.0", UIMin = "20.0"))
	float CellSize;

	// 1D array representing the 2D Maze (contain nodes)
	UPROPERTY(BlueprintReadOnly)
	TArray<FNode> MazeMap;

public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

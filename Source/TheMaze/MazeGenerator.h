// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeDataEnum.h"
#include "MazeDataStruct.h"
#include "MazeGameInstance.h"
#include "MonsterAI.h"
#include "TriggerSpikes.h"
#include "DoorObject.h"
#include "KeyItem.h"
#include "HealthPackItem.h"
#include "EscapeLadder.h"
#include "TheMazeCharacter.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class THEMAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()

private:
	// Test
	UStaticMeshComponent* SphereMesh;

	// Max number of dead end for keys and doors
	int MaxDeadEnd;
	
	// Position of the origin point
	FVector2D Origin;

	// Position of the next origin point
	FVector2D NextOrigin;

	// Array of possible direction
	TArray<FVector2D> PossibleDirection;

	/** Maze game instance */
	TObjectPtr<UMazeGameInstance> MazeGI;

	// Scene Component
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// List of MonsterAI
	TArray<AMonsterAI*> ListMonsters;

	// List of Spikes
	TArray<ATriggerSpikes*> ListTriggerSpikes;

	// List of Objects
	TArray<AActor*> ListObjects;

	// Player start transform
	FTransform StartTransform;

	// Generate Outer walls function
	void GenerateOuterWalls();

	// Generate Walls of a Node
	void GenerateNodeWalls(FNode& CurrentNode, int CurrentWallIndex);

	// Move Wall on Z
	void MoveWallByDir(FNode& Node, EDirection dir, float ZMove);

	// Hide Wall function
	void HideWallByDir(FNode& Node, EDirection dir);

	// Show Wall function
	void ShowWallByDir(FNode& Node, EDirection dir);

	// Check if pos is in bound
	bool IsPosInBound(FVector2D pos);

	// Direction to EDirection enum
	EDirection VecToEDir(FVector2D direction);

	// Find the direction of the open wall for dead end node
	FVector2D FindDirDeadEnd(FNode* DeadEnd);

	// Update the node for dead end cases
	void UpdateNodeForDeadEnd(FNode* Node);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TEST
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Mesh")
	UInstancedStaticMeshComponent* CubeInstance;

	// NavMesh for MonsterAI navigation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|AI")
	TObjectPtr<ANavMeshBoundsVolume> NavMesh;

	// Instanced static mesh component for the floors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Mesh")
	TObjectPtr<UInstancedStaticMeshComponent> ISMFloorComponent;

	// Instanced static mesh component for the walls
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Mesh")
	TObjectPtr<UInstancedStaticMeshComponent> ISMWallComponent;

	// AI Monster blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<AMonsterAI> MonsterAIBP;

	// Trigger Spikes blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<ATriggerSpikes> TriggerSpikesBP;

	// Door blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<ADoorObject> DoorBP;

	// Key blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<AKeyItem> KeyBP;

	// Health Pack blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<AHealthPackItem> HealthPackBP;

	// Ladder blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Spawnable")
	TSubclassOf<AEscapeLadder> LadderBP;

	// Width of the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int Width;

	// Height of the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int Height;

	// Number of Monster in the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int NumberOfMonster;

	// Time between Maze Evolution
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int EvolutionTime;

	// Number of Rare key to escape the Maze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Settings")
	int NumberOfRareKey;

	// Probability that a Trigger Spike spawn in the Maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze|Settings")
	float ProbaTriggerSpikes;

	// Probability that a Health Pack spawn in the Maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze|Settings")
	float ProbaHealthPack;

	// List of the total number of key for each Tier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze|Settings")
	TArray<int> ListNumberKeyByTier;

	// Size of each cell of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze|Settings", meta = (ClampMin = "20.0", UIMin = "20.0"))
	float CellSize = 200;

	// 1D array representing the 2D Maze (contain nodes)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TArray<FNode> MazeMap;

	// List of Unpopulated Dead End Node
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TArray<FNode> ListUnpopulatedDeadEnd;

	// List of Populated Dead End Node
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TArray<FNode> ListPopulatedDeadEnd;

public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Generate a new Maze map (default map)
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void NewMazeMap();

	// One step iteration of the maze generation
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void MazeGenIteration();

	// Randomize the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void MazeGenMultiStepRandomize();

	// Make the maze evolve
	UFUNCTION(BlueprintCallable, Category = "Maze")
	void MazeGenEvolution();

	// Spawn Start and End
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze")
	void SpawnStartEnd();

	// Get the index of a neighbour node via direction
	UFUNCTION(BlueprintCallable, Category = "Maze")
	int GetNeighbourViaDirection(FVector2D pos, FVector2D dir) const { return (pos.X + dir.X) + (pos.Y + dir.Y) * Width; }

	// Spawn the Monster AI in the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void MonsterAISpawn();

	// Clear all the Monster AI in the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void MonsterAIClear();

	// Spawn the Trigger Spikes in the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void TriggerSpikesSpawn();

	// Clear all the Trigger Spikes in the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void TriggerSpikesClear();

	// Spawn the Health Pack in the maze
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	void HealthPackSpawn();

	// Spawn Doors and Keys in the Maze
	UFUNCTION(CallInEditor, Category = "Maze")
	void SpawnKeyDoor();

	// Clear all the items in the Maze
	UFUNCTION(CallInEditor, Category = "Maze")
	void ClearObjects();

	// Spawn the Monster AI in the maze
	UFUNCTION(BlueprintCallable, Category = "Maze")
	void KeySpawn(EKeyDoorTier Tier);

	// Called to Spawn the Player at the Start Room
	UFUNCTION(BlueprintCallable, Category = "Maze")
	void SpawnPlayer();

	// Display Dead End
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze")
	void DisplayDeadEnd();

	// Clear all the Monster AI in the maze
	// UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze") // Just for testing
	// void KeyClear();
};

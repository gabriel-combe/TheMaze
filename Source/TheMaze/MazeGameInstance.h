// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MazeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API UMazeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Size of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	TArray<int> Size = {15, 15};

	// Number of enemies in the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int NBEnemies = 2;

	// Time to complete the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int ChronoTime = 180;

	// Time between maze evolution
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int EvolutionTime = 10;

	// Number of Rare key to complete the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int NBRareKey = 3;

	// Proba to spawn a trigger spike in the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	float ProbaTriggerSpikes = 0.1f;

	// Proba to spawn a health pack in the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	float ProbaHealthPack = 0.1f;
};

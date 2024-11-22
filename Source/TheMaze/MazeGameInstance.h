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
	
protected:
	UMazeGameInstance();
	
public:
	// Size of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	TArray<int> Size;

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
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	TArray<int> Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int NBEnemies = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int ChronoTime = 60;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"


UENUM(BlueprintType)
enum class EKeyDoorTier : uint8 {
	KeyDoor_Common		UMETA(DisplayName = "Common"),
	KeyDoor_Uncommon	UMETA(DisplayName = "Uncommon"),
	KeyDoor_Rare		UMETA(DisplayName = "Rare"),
};

/**
 * 
 */
//class THEMAZE_API MazeData
//{
//public:
//	MazeData();
//	~MazeData();
//};

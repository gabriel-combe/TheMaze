// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDInterface.h"
#include "GameFramework/HUD.h"
#include "MazeHUD.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API AMazeHUD : public AHUD, public IHUDInterface
{
	GENERATED_BODY()

public:
	// Implement the Activate pause function of the interface
	void ActivatePause_Implementation() override;

	// Implement the Activate pause function of the interface
	void DeactivatePause_Implementation() override;
	
};

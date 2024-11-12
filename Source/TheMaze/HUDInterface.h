// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEMAZE_API IHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Called when the player activate the pause menu
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player|Menu")
	void ActivatePause();

	// Called when the player deactivate the pause menu
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player|Menu")
	void DeactivatePause();
};

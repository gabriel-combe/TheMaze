// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheMazeGameMode.h"
#include "TheMazeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheMazeGameMode::ATheMazeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// set defqult hud class to our Blueprinted hud
	// static ConstructorHelpers::FClassFinder<> 

}

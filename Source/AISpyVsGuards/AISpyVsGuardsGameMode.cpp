// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AISpyVsGuardsGameMode.h"
#include "AISpyVsGuardsPlayerController.h"
#include "AISpyVsGuardsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAISpyVsGuardsGameMode::AAISpyVsGuardsGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAISpyVsGuardsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
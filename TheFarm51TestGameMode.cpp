// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TheFarm51TestGameMode.h"
#include "TheFarm51TestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HUDController.h"
#include "AHUD.h"

ATheFarm51TestGameMode::ATheFarm51TestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//static ConstructorHelpers::FObjectFinder<UWidgetComponent> GameHUDComponent(TEXT("/Game/HUD/GameHUD"));
	//HUD = GameHUDComponent.Object;
	//PlayerControllerClass = AHUDController::StaticClass();
	//HUDClass = AHUD::StaticClass();
}
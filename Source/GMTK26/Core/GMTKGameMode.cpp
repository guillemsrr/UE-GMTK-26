// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKGameMode.h"

#include "GMTKPlayerController.h"
#include "Player/GMTKPawn.h"
#include "UI/GMTKHUD.h"
#include "UObject/ConstructorHelpers.h"

AGMTKGameMode::AGMTKGameMode()
{
	DefaultPawnClass = AGMTKPawn::StaticClass();
	PlayerControllerClass = AGMTKPlayerController::StaticClass();
	HUDClass = AGMTKHUD::StaticClass();
}

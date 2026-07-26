// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKPlayerController.h"

#include "Components/InputComponent.h"
#include "GMTKGameMode.h"

#include "Engine/World.h"

AGMTKPlayerController::AGMTKPlayerController()
{
	bShowMouseCursor = true;
}

void AGMTKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto BindMenuKey = [this](const FKey& Key)
	{
		FInputKeyBinding& Binding = InputComponent->BindKey(Key, IE_Pressed, this, &AGMTKPlayerController::ToggleMenu);
		Binding.bConsumeInput = false;
	};

	BindMenuKey(EKeys::Escape);
	BindMenuKey(EKeys::M);
}

void AGMTKPlayerController::ToggleMenu()
{
	GetWorld()->GetAuthGameMode<AGMTKGameMode>()->ToggleMenu();
}

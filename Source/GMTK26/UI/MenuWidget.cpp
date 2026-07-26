// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MenuWidget.h"

#include "Core/GMTKGameMode.h"
#include "GameFramework/PlayerController.h"
#include "MenuButtonWidget.h"

#include "Engine/World.h"

UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddUObject(this, &UMenuWidget::OnContinueClicked);
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddUObject(this, &UMenuWidget::OnRestartClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddUObject(this, &UMenuWidget::OnMainMenuClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUObject(this, &UMenuWidget::OnQuitClicked);
	}
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetOwningPlayer();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;

	TakeKeyboardFocus();
}

void UMenuWidget::NativeDestruct()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeGameOnly GameInputMode;
		GameInputMode.SetConsumeCaptureMouseDown(false);
		PlayerController->SetInputMode(GameInputMode);
		PlayerController->bShowMouseCursor = true;
	}

	Super::NativeDestruct();
}

FReply UMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Escape || Key == EKeys::M)
	{
		GetGameMode()->ToggleMenu();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

AGMTKGameMode* UMenuWidget::GetGameMode() const
{
	return GetWorld()->GetAuthGameMode<AGMTKGameMode>();
}

void UMenuWidget::OnContinueClicked()
{
	GetGameMode()->StartRun();
}

void UMenuWidget::OnRestartClicked()
{
	GetGameMode()->RestartRun();
}

void UMenuWidget::OnMainMenuClicked()
{
	GetGameMode()->ReturnToMainMenu();
}

void UMenuWidget::OnQuitClicked()
{
	GetGameMode()->QuitGame();
}

void UMenuWidget::TakeKeyboardFocus()
{
	for (UMenuButtonWidget* MenuButton : {
		     ToRawPtr(ContinueButton), ToRawPtr(RestartButton), ToRawPtr(MainMenuButton), ToRawPtr(QuitButton)
	     })
	{
		if (MenuButton)
		{
			MenuButton->FocusButton();
			return;
		}
	}

	SetKeyboardFocus();
}

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKHUD.h"

#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.h"
#include "GameFramework/PlayerController.h"
#include "MenuWidget.h"
#include "PauseMenuWidget.h"
#include "TutorialWidget.h"

namespace
{
	template <typename WidgetType>
	WidgetType* MakeWidget(APlayerController* Owner, TObjectPtr<WidgetType>& Widget,
	                       const TSubclassOf<WidgetType>& WidgetClass)
	{
		if (!Widget && WidgetClass)
		{
			Widget = CreateWidget<WidgetType>(Owner, WidgetClass);
		}

		return Widget;
	}

	void AddToViewportOnce(UUserWidget* Widget)
	{
		if (Widget && !Widget->IsInViewport())
		{
			Widget->AddToViewport();
		}
	}
}

void AGMTKHUD::ShowMainMenu()
{
	OpenMenu(MakeWidget(GetOwningPlayerController(), MainMenu, MainMenuClass));
}

void AGMTKHUD::ShowPauseMenu()
{
	OpenMenu(MakeWidget(GetOwningPlayerController(), PauseMenu, PauseMenuClass));
}

void AGMTKHUD::ShowEndScreen(bool bPlayerWon)
{
	UEndScreenWidget* Screen = MakeWidget(GetOwningPlayerController(), EndScreen, EndScreenClass);
	if (!Screen)
	{
		return;
	}

	Screen->SetOutcome(bPlayerWon);

	OpenMenu(Screen);
}

void AGMTKHUD::CloseMenu()
{
	if (ActiveMenu)
	{
		ActiveMenu->RemoveFromParent();
		ActiveMenu = nullptr;
	}

	UpdateRunWidgets();
}

bool AGMTKHUD::CanCloseMenu() const
{
	return ActiveMenu && ActiveMenu->IsDismissable();
}

void AGMTKHUD::ShowTutorial(AActor* Source, const TArray<FString>& InLines)
{
	TutorialSource = Source;
	TutorialLines = InLines;

	if (UTutorialWidget* Panel = MakeWidget(GetOwningPlayerController(), TutorialWidget, TutorialWidgetClass))
	{
		Panel->SetLines(TutorialLines);
		AddToViewportOnce(Panel);
	}

	UpdateRunWidgets();
}

void AGMTKHUD::HideTutorial(AActor* Source)
{
	if (TutorialSource != Source)
	{
		return;
	}

	TutorialSource = nullptr;
	TutorialLines.Reset();

	UpdateRunWidgets();
}

void AGMTKHUD::OpenMenu(UMenuWidget* Menu)
{
	if (!Menu)
	{
		return;
	}

	CloseMenu();

	ActiveMenu = Menu;
	Menu->AddToViewport(MenuZOrder);

	UpdateRunWidgets();
}

void AGMTKHUD::UpdateRunWidgets()
{
	const bool bPlaying = !IsMenuOpen();
	if (bPlaying)
	{
		AddToViewportOnce(MakeWidget(GetOwningPlayerController(), HintWidget, HintWidgetClass));
	}

	if (HintWidget)
	{
		HintWidget->SetVisibility(bPlaying ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(bPlaying && !TutorialLines.IsEmpty()
			                              ? ESlateVisibility::HitTestInvisible
			                              : ESlateVisibility::Collapsed);
	}
}

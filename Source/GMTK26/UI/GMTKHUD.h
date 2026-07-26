// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GMTKHUD.generated.h"

class UUserWidget;
class UEndScreenWidget;
class UMenuWidget;
class UPauseMenuWidget;
class UTutorialWidget;

UCLASS()
class GMTK26_API AGMTKHUD : public AHUD
{
	GENERATED_BODY()

public:
	void ShowMainMenu();
	void ShowPauseMenu();
	void ShowEndScreen(bool bPlayerWon);

	void CloseMenu();

	bool IsMenuOpen() const
	{
		return ActiveMenu != nullptr;
	}

	bool CanCloseMenu() const;

	void ShowTutorial(AActor* Source, const TArray<FString>& InLines);
	void HideTutorial(AActor* Source);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> MainMenuClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEndScreenWidget> EndScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTutorialWidget> TutorialWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HintWidgetClass;

private:
	void OpenMenu(UMenuWidget* Menu);

	void UpdateRunWidgets();

	UPROPERTY()
	TObjectPtr<UMenuWidget> MainMenu;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenu;

	UPROPERTY()
	TObjectPtr<UEndScreenWidget> EndScreen;

	UPROPERTY()
	TObjectPtr<UMenuWidget> ActiveMenu;

	UPROPERTY()
	TObjectPtr<UTutorialWidget> TutorialWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> HintWidget;

	UPROPERTY()
	TObjectPtr<AActor> TutorialSource;

	TArray<FString> TutorialLines;
};

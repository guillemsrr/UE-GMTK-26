// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class AGMTKGameMode;
class UMenuButtonWidget;

UCLASS(Abstract)
class GMTK26_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool IsDismissable() const
	{
		return false;
	}

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	AGMTKGameMode* GetGameMode() const;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButtonWidget> ContinueButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButtonWidget> RestartButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButtonWidget> MainMenuButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButtonWidget> QuitButton;

private:
	void OnContinueClicked();
	void OnRestartClicked();
	void OnMainMenuClicked();
	void OnQuitClicked();
	void TakeKeyboardFocus();
};

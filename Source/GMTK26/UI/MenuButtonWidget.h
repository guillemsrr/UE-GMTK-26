// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS(Abstract)
class GMTK26_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnMenuButtonClicked);

	FOnMenuButtonClicked OnClicked;
	void FocusButton();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(EditAnywhere)
	FText Label = INVTEXT("BUTTON");

private:
	UFUNCTION()
	void HandleButtonClicked();
};

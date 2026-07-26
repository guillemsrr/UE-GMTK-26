// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MenuButtonWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMenuButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &UMenuButtonWidget::HandleButtonClicked);
}

void UMenuButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	LabelText->SetText(Label);
}

void UMenuButtonWidget::FocusButton()
{
	Button->SetKeyboardFocus();
}

void UMenuButtonWidget::HandleButtonClicked()
{
	OnClicked.Broadcast();
}

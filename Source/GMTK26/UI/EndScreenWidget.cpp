// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "EndScreenWidget.h"

#include "Components/TextBlock.h"

void UEndScreenWidget::SetOutcome(bool bPlayerWon)
{
	TitleText->SetText(bPlayerWon ? WonTitle : LostTitle);
	TitleText->SetColorAndOpacity(bPlayerWon ? WonColor : LostColor);
}

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "TutorialWidget.h"

#include "Components/TextBlock.h"

void UTutorialWidget::SetLines(const TArray<FString>& Lines)
{
	TutorialText->SetText(FText::FromString(FString::Join(Lines, TEXT("\n"))));
}

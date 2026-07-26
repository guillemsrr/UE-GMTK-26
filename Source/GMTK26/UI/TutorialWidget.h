// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

class UTextBlock;

UCLASS(Abstract)
class GMTK26_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLines(const TArray<FString>& Lines);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TutorialText;
};

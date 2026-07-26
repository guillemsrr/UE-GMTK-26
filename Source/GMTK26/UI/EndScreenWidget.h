// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "EndScreenWidget.generated.h"

class UTextBlock;

UCLASS(Abstract)
class GMTK26_API UEndScreenWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	void SetOutcome(bool bPlayerWon);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(EditAnywhere)
	FText WonTitle = INVTEXT("YOU ESCAPED");

	UPROPERTY(EditAnywhere)
	FText LostTitle = INVTEXT("OUT OF LIVES");

	UPROPERTY(EditAnywhere)
	FLinearColor WonColor = FLinearColor(0.3f, 1.0f, 0.4f);

	UPROPERTY(EditAnywhere)
	FLinearColor LostColor = FLinearColor(1.0f, 0.2f, 0.2f);
};

// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS(Abstract)
class GMTK26_API UPauseMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool IsDismissable() const override
	{
		return true;
	}
};

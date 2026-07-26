// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GMTKPlayerController.generated.h"

UCLASS(Abstract)
class GMTK26_API AGMTKPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGMTKPlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	void ToggleMenu();
};

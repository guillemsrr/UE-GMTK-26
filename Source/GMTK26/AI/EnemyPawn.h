// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "Player/GMTKPawnBase.h"

#include "EnemyPawn.generated.h"

UCLASS()
class GMTK26_API AEnemyPawn : public AGMTKPawnBase
{
	GENERATED_BODY()

protected:
	AEnemyPawn();
	virtual void BeginPlay() override;
};

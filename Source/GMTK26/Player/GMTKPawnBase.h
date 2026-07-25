// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GMTKPawnBase.generated.h"

class UFloatingPawnMovement;
class UGMTKNeonComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API AGMTKPawnBase : public APawn
{
	GENERATED_BODY()

protected:
	AGMTKPawnBase();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGMTKNeonComponent> NeonLightComponent;
};

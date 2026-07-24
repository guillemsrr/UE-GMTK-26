// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ALocker;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API ADoor : public AActor
{
	GENERATED_BODY()

protected:
	ADoor();
	virtual void BeginPlay() override;

public:
	ALocker* GetLocker() const
	{
		return Locker;
	}

	void Open();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<ALocker> Locker;
};

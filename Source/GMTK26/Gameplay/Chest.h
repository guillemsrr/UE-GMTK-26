// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

class AGMTKPawn;
class AMinionLife;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API AChest : public AActor
{
	GENERATED_BODY()

public:
	AChest();

	bool IsOpen() const { return bIsOpen; }

	// Spawns the starting minions in a ring around the chest and hands them to the opener. Only pays out once.
	void Open(AActor* Opener);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMinionLife> MinionClass;

	UPROPERTY(EditAnywhere)
	int32 MinionCount = 3;

	// Minions pop out on this ring so they do not all spawn inside each other.
	UPROPERTY(EditAnywhere)
	float SpawnRadius = 120.0f;

	UPROPERTY(EditAnywhere)
	float SpawnHeight = 60.0f;

private:
	bool bIsOpen = false;
};

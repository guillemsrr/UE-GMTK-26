// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "Chest.generated.h"

class AEnemyPawn;
class AGMTKPawn;
class UGMTKNeonComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API AChest : public AActor
{
	GENERATED_BODY()

public:
	AChest();

	bool IsOpen() const
	{
		return bIsOpen;
	}

	void Open(AActor* Opener);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGMTKNeonComponent> NeonLightComponent;

	UPROPERTY(EditAnywhere)
	int32 MinionCount = 3;

	// Minions pop out on this ring so they do not all spawn inside each other.
	UPROPERTY(EditAnywhere)
	float SpawnRadius = 120.0f;

	UPROPERTY(EditAnywhere)
	float SpawnHeight = 60.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyPawn> EnemyClass;

	UPROPERTY(EditAnywhere)
	int32 EnemyCount = 0;

	// Outside the minion ring, so the reward is never spawned inside whatever comes out with it.
	UPROPERTY(EditAnywhere)
	float EnemySpawnRadius = 260.0f;

	UPROPERTY(EditAnywhere)
	float EnemySpawnHeight = 0.0f;

private:
	bool bIsOpen = false;

	void SpawnEnemies();
};

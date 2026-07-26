// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Locker.generated.h"

class ADoor;
class AGMTKPawn;
class AMinionLife;
class UGMTKNeonComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API ALocker : public AActor
{
	GENERATED_BODY()

protected:
	ALocker();
	virtual void BeginPlay() override;

public:
	bool IsFilled() const
	{
		return bFilled;
	}

	bool IsAvailable() const
	{
		return !bFilled && !bClaimed;
	}

	void Claim()
	{
		bClaimed = true;
	}

	void ReleaseClaim()
	{
		bClaimed = false;
	}

	FVector GetSocketLocation() const;

	void InsertMinion(AMinionLife* Minion);

	bool RetrieveMinion(AGMTKPawn* Receiver);

	void SetDoor(ADoor* InDoor)
	{
		Door = InDoor;
	}

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGMTKNeonComponent> NeonLightComponent;

	UPROPERTY(EditAnywhere)
	int32 RewardMinionCount = 0;

	UPROPERTY(EditAnywhere)
	float RewardSpawnRadius = 180.0f;

	UPROPERTY(EditAnywhere)
	float RewardSpawnHeight = 60.0f;

private:
	UPROPERTY()
	TObjectPtr<ADoor> Door;

	UPROPERTY()
	TObjectPtr<AMinionLife> StoredMinion;

	bool bFilled = false;
	bool bClaimed = false;
	bool bRewardClaimed = false;
	FVector RetrievalSideLocation = FVector::ZeroVector;
};

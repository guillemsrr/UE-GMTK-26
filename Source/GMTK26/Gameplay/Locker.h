// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Locker.generated.h"

class ADoor;
class AMinionLife;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API ALocker : public AActor
{
	GENERATED_BODY()

protected:
	ALocker();
	virtual void BeginPlay() override;

public:
	bool IsUnlocked() const
	{
		return InsertedCount >= RequiredMinions;
	}

	// Sockets nobody has been sent to yet. Claimed-but-unfilled sockets do not count, so two minions
	// never race for the last one.
	int32 GetFreeSocketCount() const
	{
		return RequiredMinions - InsertedCount - ClaimedCount;
	}

	// Reserves the next socket for an inbound minion and returns its index, or INDEX_NONE when full.
	int32 ClaimSocket();

	// Hands a claim back when the minion never made it.
	void ReleaseSocket();

	FVector GetSocketLocation(int32 SocketIndex) const;

	// Consumes the minion: its sphere is removed from the countdown and the minion is spent for good.
	void InsertMinion(AMinionLife* Minion, int32 SocketIndex);

	void SetDoor(ADoor* InDoor)
	{
		Door = InDoor;
	}

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	int32 RequiredMinions = 3;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SocketSphereMesh;

	UPROPERTY(EditAnywhere)
	float SocketSphereScale = 0.15f;

	UPROPERTY(EditAnywhere)
	float SocketSpacing = 70.0f;

	// Measured from the actor origin, so it has to clear whatever mesh the locker wears.
	UPROPERTY(EditAnywhere)
	float SocketHeight = 200.0f;

private:
	// One sphere per socket, indexed the same way. Entries go null as sockets are filled.
	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> SocketSpheres;

	UPROPERTY()
	TObjectPtr<ADoor> Door;

	int32 InsertedCount = 0;
	int32 ClaimedCount = 0;
};

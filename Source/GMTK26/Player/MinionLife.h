// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GMTKPawnBase.h"

#include "MinionLife.generated.h"

class ALocker;

UCLASS(Abstract)
class GMTK26_API AMinionLife : public AGMTKPawnBase
{
	GENERATED_BODY()

protected:
	AMinionLife();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetFollowTarget(AActor* NewTarget);
	void SetOrbitSlot(int32 SlotIndex, int32 SlotCount);

	// Leaves the orbit for good and flies to the socket it was promised, which then consumes it.
	void DeployToSocket(ALocker* Locker, int32 SocketIndex);

protected:
	UPROPERTY(EditAnywhere)
	float OrbitRadius = 160.0f;

	UPROPERTY(EditAnywhere)
	float OrbitHeight = 40.0f;

	// Degrees per second the slot travels around the target.
	UPROPERTY(EditAnywhere)
	float OrbitAngularSpeed = 60.0f;

	// Inside this distance the minion coasts, so it settles on the slot instead of jittering across it.
	UPROPERTY(EditAnywhere)
	float ArrivalRadius = 15.0f;

	// Steering input fades out over this distance, which keeps the trail springy without overshooting.
	UPROPERTY(EditAnywhere)
	float BrakingDistance = 200.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> FollowTarget;

private:
	float OrbitAngle = 0.0f;

	UPROPERTY()
	TObjectPtr<ALocker> DeployLocker;

	int32 DeploySocketIndex = INDEX_NONE;

	FVector GetSlotLocation() const;

	// Steers toward Destination and reports whether it has arrived.
	bool SteerTo(const FVector& Destination);
};

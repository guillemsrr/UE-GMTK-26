// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GMTKPawnBase.h"

#include "MinionLife.generated.h"

class AChest;
class AEnemyPawn;
class ALocker;
class UMinionComponent;

// Where a sphere is in its short life. There is no "attacking" or "opening" state because the job is
// whatever happened to be standing next to it, and it can change its mind the moment that thing dies.
enum class EMinionMode : uint8
{
	Orbit,
	Deployed,
	// Flying into a socket. Still a life until it lands, so it can still be called back.
	Spending,
	Stored
};

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
	AActor* GetFollowTarget() const
	{
		return FollowTarget;
	}

	void SetOrbitSlot(int32 SlotIndex, int32 SlotCount);
	void SetNeonColor(const FLinearColor& Color);

	bool IsOrbiting() const
	{
		return Mode == EMinionMode::Orbit;
	}

	bool IsStored() const
	{
		return Mode == EMinionMode::Stored;
	}

	// Sends the sphere to a spot and leaves it to find its own work there.
	void DeployTo(const FVector& Location);

	// Enemy input has no cursor: this gives its sphere a central pawn to chase directly.
	void DeployAgainst(AActor* Target);

	// Drops whatever it was doing and rejoins the orbit. Safe to call on a sphere already home.
	void Recall();

	// Holds visibly in a filled locker without steering or looking for work.
	void StoreAt(const FVector& Location);

protected:
	UPROPERTY(EditAnywhere)
	float OrbitRadius = 160.0f;

	UPROPERTY(EditAnywhere)
	float OrbitHeight = 40.0f;

	// How wide the spheres fan out around the aim direction. 360 gives a plain ring; anything less reads
	// as a formation pointed at the cursor, which is the whole point of hanging it off the mouse.
	UPROPERTY(EditAnywhere)
	float OrbitArc = 180.0f;

	// How hard the sphere chases its slot as the cursor swings, per second. Low enough that the trail
	// sweeps around behind the pointer instead of teleporting with it.
	UPROPERTY(EditAnywhere)
	float OrbitTurnSpeed = 5.0f;

	// Inside this distance the minion coasts, so it settles on the slot instead of jittering across it.
	UPROPERTY(EditAnywhere)
	float ArrivalRadius = 15.0f;

	// Steering input fades out over this distance, which keeps the trail springy without overshooting.
	UPROPERTY(EditAnywhere)
	float BrakingDistance = 200.0f;

	// How far a deployed sphere looks for something to do. Big enough that dropping one "on" a door or
	// a triangle is a loose gesture, small enough that it never reaches into the next room.
	UPROPERTY(EditAnywhere)
	float WorkRadius = 320.0f;

	// The world is scanned on a beat rather than every frame: work does not appear that fast, and a
	// handful of spheres each sweeping every actor would.
	UPROPERTY(EditAnywhere)
	float WorkScanInterval = 0.2f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> FollowTarget;

private:
	EMinionMode Mode = EMinionMode::Orbit;

	// Where this sphere sits within the fan, as a signed offset from the aim direction.
	float SlotAngle = 0.0f;

	// Where it actually is right now, easing toward the slot. Kept separate so a sphere that spawns or
	// comes home somewhere odd swings in from there rather than popping into place.
	float OrbitAngle = 0.0f;

	// Where the player pointed. Held onto so a sphere that finishes a fight goes back to waiting there
	// instead of drifting after the corpse.
	FVector DeployLocation = FVector::ZeroVector;

	float WorkScanCooldown = 0.0f;

	UPROPERTY()
	TObjectPtr<ALocker> DeployLocker;

	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY()
	TObjectPtr<UMinionComponent> FollowMinions;

	void TickOrbit(float DeltaTime);
	void TickDeployed(float DeltaTime);
	void TickLocker();
	bool TryInterceptOrb();
	void Consume();

	// Picks up whatever is nearest, whatever kind of thing it is. Returns false when there is nothing
	// worth doing in range.
	bool FindWork();

	FVector GetSlotLocation() const;

	// Steers toward Destination and reports whether it has arrived.
	bool SteerTo(const FVector& Destination);
};

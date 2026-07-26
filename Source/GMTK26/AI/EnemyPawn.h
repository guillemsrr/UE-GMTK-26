// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Player/GMTKPawnBase.h"

#include "EnemyPawn.generated.h"

class AGMTKPawn;
class AMinionLife;
class UMinionComponent;
class UTextRenderComponent;

// The two beats of the only enemy in the game. Everything else — where it stands, how many of them
// share a room, how far apart their wind-ups start — is level design, not a new state.
enum class EEnemyState : uint8
{
	WindUp,
	Recovering
};

// One enemy, no behaviour tree: it owns the same spheres as the player, winds up, launches one, then
// recovers and repeats. Its orbit is both ammunition and a physical shield.
UCLASS()
class GMTK26_API AEnemyPawn : public AGMTKPawnBase
{
	GENERATED_BODY()

public:
	AEnemyPawn();

	bool IsAlive() const
	{
		return !IsActorBeingDestroyed();
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	int32 InitialMinionCount = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMinionLife> MinionClass;

	UPROPERTY(EditAnywhere)
	float MinionSpawnRadius = 160.0f;

	UPROPERTY(EditAnywhere)
	float MinionSpawnHeight = 40.0f;

	UPROPERTY(EditAnywhere)
	FLinearColor MinionColor = FLinearColor(1.0f, 0.08f, 0.03f);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMinionComponent> MinionComponent;

	// How many spheres this enemy is still carrying, written over its head. The orbit says it too, but
	// only once the player is close enough to count overlapping dots — this is readable across the room,
	// which is where the decision of who to take on first is actually made.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextRenderComponent> MinionCountText;

	// Outside this range the countdown is frozen, which keeps a room's enemies asleep until the player
	// walks in and stops distant rooms from firing through a wall.
	UPROPERTY(EditAnywhere)
	float AggroRadius = 1400.0f;

	UPROPERTY(EditAnywhere)
	float WindUpTime = 2.0f;

	// Head start on the first wind-up only. Staggering it across a room is what makes two identical
	// enemies feel like a pattern instead of a wall.
	UPROPERTY(EditAnywhere)
	float WindUpOffset = 0.0f;

	UPROPERTY(EditAnywhere)
	float RecoveryTime = 0.9f;

	UPROPERTY(EditAnywhere)
	float WindUpScale = 1.35f;

	UPROPERTY(EditAnywhere)
	FLinearColor RestingColor = FLinearColor(0.35f, 0.02f, 0.02f);

	UPROPERTY(EditAnywhere)
	FLinearColor ReadyColor = FLinearColor(1.0f, 0.25f, 0.05f);

private:
	EEnemyState State = EEnemyState::WindUp;
	float StateTime = 0.0f;

	FVector BaseMeshScale = FVector::OneVector;

	UPROPERTY()
	TObjectPtr<AGMTKPawn> Player;

	void EnterState(EEnemyState NewState);
	void HandleMinionCountChanged(int32 NewCount);
	void HandleMinionsDepleted();
	void HandleDirectHit();

	// The count is flat text on a fixed-angle camera, so it has to be turned to face the view every frame
	// or it disappears the moment the enemy turns to aim.
	void FaceCountTextAtCamera();

	// 0 at the start of the wind-up, 1 the instant it fires.
	float GetWindUpRatio() const;

	void UpdateTell(float WindUpRatio);
};

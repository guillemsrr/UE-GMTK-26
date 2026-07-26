// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Engine/TimerHandle.h"
#include "GMTKPawnBase.h"

#include "GMTKPawn.generated.h"

class AMinionLife;
class UAbilitySystemComponent;
class UGameplayAbility;
class UInputAction;
class UInputMappingContext;
class UMinionComponent;
struct FInputActionValue;

UCLASS(Abstract)
class GMTK26_API AGMTKPawn : public AGMTKPawnBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	AGMTKPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	int32 GetMinionCount() const;

	UMinionComponent* GetMinionComponent() const
	{
		return MinionComponent;
	}

	TSubclassOf<AMinionLife> GetMinionClass() const
	{
		return MinionClass;
	}

	// Sends the sphere nearest the point out to it. False when every sphere is already out there.
	bool DeployMinion(const FVector& Location);

	// Calls every sphere home, including ones flying toward or already held in a socket.
	void RetrieveMinions();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMinionComponent> MinionComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMinionLife> MinionClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> DeployAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> RetrieveAction;

	// Seconds between spheres while the deploy button is held. Holding is the cost: sending three
	// somewhere takes three times as long as sending one, so a mistake is felt while it is made.
	UPROPERTY(EditAnywhere)
	float DeployInterval = 0.5f;

	UPROPERTY(EditAnywhere)
	float MinimumMovementSpeed = 450.0f;

	UPROPERTY(EditAnywhere)
	float MaximumMovementSpeed = 900.0f;

	// One sphere uses the minimum speed. Reaching this many gives the full speed, and extras do not
	// make the pawn faster still.
	UPROPERTY(EditAnywhere)
	int32 MinionsForMaximumMovementSpeed = 5;

	virtual void Move(const FInputActionValue& Value);

	// Press deploys one immediately, then the timer keeps them going out for as long as it is held.
	virtual void StartDeploying();
	virtual void StopDeploying();
	virtual void Retrieve();

private:
	FTimerHandle DeployTimer;

	// Kept from the last frame the cursor was over something, so the orbit holds its heading instead of
	// snapping back when the pointer slides off the level.
	float AimYaw = 0.0f;

	void Deploy();
	void HandleMinionCountChanged(int32 MinionCount);
	void HandleMinionsDepleted();
	void HandleDirectHit();
	void UpdateAimYaw();
};

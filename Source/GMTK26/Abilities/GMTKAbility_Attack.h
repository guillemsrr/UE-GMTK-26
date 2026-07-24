// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GMTKAbility_Attack.generated.h"

class AChest;

// The "destroy" verb: hits whatever destructible is in range. Chests are the only target type so far;
// enemies join once they carry health. One ability owns the choice so a single press never resolves twice.
UCLASS()
class GMTK26_API UGMTKAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGMTKAbility_Attack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 300.0f;

private:
	AChest* FindChestInRange(const AActor* Avatar) const;
};

// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"

#include "GMTKAbility_Dash.generated.h"

UCLASS()
class GMTK26_API UGMTKDashCooldown : public UGameplayEffect
{
	GENERATED_BODY()

	UGMTKDashCooldown();
	virtual void PostInitProperties() override;
};

UCLASS()
class GMTK26_API UGMTKAbility_Dash : public UGameplayAbility
{
	GENERATED_BODY()

	UGMTKAbility_Dash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};

// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GMTKAbility_Deploy.generated.h"

UCLASS()
class GMTK26_API UGMTKAbility_Deploy : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGMTKAbility_Deploy();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};

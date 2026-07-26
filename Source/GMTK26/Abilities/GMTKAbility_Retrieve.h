// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GMTKAbility_Retrieve.generated.h"

UCLASS()
class GMTK26_API UGMTKAbility_Retrieve : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGMTKAbility_Retrieve();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};

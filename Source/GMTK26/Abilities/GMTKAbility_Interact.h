// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GMTKAbility_Interact.generated.h"

class ALocker;

UCLASS()
class GMTK26_API UGMTKAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGMTKAbility_Interact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	static ALocker* ResolveLocker(AActor* HitActor);
};

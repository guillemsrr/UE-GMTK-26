// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKAbility_Retrieve.h"

#include "GMTKGameplayTags.h"
#include "Player/GMTKPawn.h"

UGMTKAbility_Retrieve::UGMTKAbility_Retrieve()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SetAssetTags(FGameplayTagContainer(GMTKGameplayTags::Ability_Retrieve.GetTag()));
}

void UGMTKAbility_Retrieve::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Cast<AGMTKPawn>(ActorInfo->AvatarActor.Get())->RetrieveMinions();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

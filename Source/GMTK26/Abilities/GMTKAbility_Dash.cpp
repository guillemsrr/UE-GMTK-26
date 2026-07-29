// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKAbility_Dash.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GMTKGameplayTags.h"
#include "Player/GMTKPawn.h"

UGMTKDashCooldown::UGMTKDashCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(2.2f));
}

void UGMTKDashCooldown::PostInitProperties()
{
	Super::PostInitProperties();

	FInheritedTagContainer CooldownTags;
	CooldownTags.AddTag(GMTKGameplayTags::Cooldown_Dash.GetTag());

	FindOrAddComponent<UTargetTagsGameplayEffectComponent>().SetAndApplyTargetTagChanges(CooldownTags);
}

UGMTKAbility_Dash::UGMTKAbility_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SetAssetTags(FGameplayTagContainer(GMTKGameplayTags::Ability_Dash.GetTag()));

	CooldownGameplayEffectClass = UGMTKDashCooldown::StaticClass();
}

void UGMTKAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Cast<AGMTKPawn>(ActorInfo->AvatarActor.Get())->StartDash();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

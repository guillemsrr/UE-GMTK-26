// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKAbility_Deploy.h"

#include "GMTKGameplayTags.h"
#include "GameFramework/PlayerController.h"
#include "Player/GMTKPawn.h"

UGMTKAbility_Deploy::UGMTKAbility_Deploy()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SetAssetTags(FGameplayTagContainer(GMTKGameplayTags::Ability_Deploy.GetTag()));
}

void UGMTKAbility_Deploy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FHitResult Hit;
	ActorInfo->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	AGMTKPawn* Pawn = Cast<AGMTKPawn>(ActorInfo->AvatarActor.Get());
	if (!Hit.bBlockingHit || !Pawn->DeployMinion(Hit.ImpactPoint))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

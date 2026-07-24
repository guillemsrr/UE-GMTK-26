// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKAbility_Interact.h"

#include "GMTKGameplayTags.h"
#include "Gameplay/Door.h"
#include "Gameplay/Locker.h"
#include "GameFramework/PlayerController.h"
#include "Player/GMTKPawn.h"

UGMTKAbility_Interact::UGMTKAbility_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SetAssetTags(FGameplayTagContainer(GMTKGameplayTags::Ability_Interact.GetTag()));
}

void UGMTKAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	ALocker* Locker = ResolveLocker(Hit.GetActor());
	AGMTKPawn* Pawn = Cast<AGMTKPawn>(ActorInfo->AvatarActor.Get());
	if (!Locker || !Pawn->SendMinionToLocker(Locker))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

ALocker* UGMTKAbility_Interact::ResolveLocker(AActor* HitActor)
{
	if (ALocker* Locker = Cast<ALocker>(HitActor))
	{
		return Locker;
	}

	if (const ADoor* Door = Cast<ADoor>(HitActor))
	{
		return Door->GetLocker();
	}

	return nullptr;
}

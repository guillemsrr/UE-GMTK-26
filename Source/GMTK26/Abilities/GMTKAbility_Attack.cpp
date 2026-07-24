// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKAbility_Attack.h"

#include "EngineUtils.h"
#include "GMTKGameplayTags.h"
#include "Gameplay/Chest.h"
#include "Player/GMTKPawn.h"

UGMTKAbility_Attack::UGMTKAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SetAssetTags(FGameplayTagContainer(GMTKGameplayTags::Ability_Attack.GetTag()));
}

void UGMTKAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AChest* Chest = FindChestInRange(ActorInfo->AvatarActor.Get());
	if (!Chest)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Chest->Open(ActorInfo->AvatarActor.Get());

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

AChest* UGMTKAbility_Attack::FindChestInRange(const AActor* Avatar) const
{
	const FVector Origin = Avatar->GetActorLocation();

	AChest* Nearest = nullptr;
	float NearestDistanceSquared = FMath::Square(AttackRange);

	for (TActorIterator<AChest> It(Avatar->GetWorld()); It; ++It)
	{
		AChest* Chest = *It;
		if (Chest->IsOpen())
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(Origin, Chest->GetActorLocation());
		if (DistanceSquared <= NearestDistanceSquared)
		{
			Nearest = Chest;
			NearestDistanceSquared = DistanceSquared;
		}
	}

	return Nearest;
}

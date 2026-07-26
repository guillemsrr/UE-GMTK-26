// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "Locker.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GMTKPawn.h"
#include "Player/MinionComponent.h"
#include "Player/MinionLife.h"
#include "Rendering/GMTKNeonComponent.h"

ALocker::ALocker()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetupAttachment(SceneRoot);

	NeonLightComponent = CreateDefaultSubobject<UGMTKNeonComponent>(TEXT("NeonLightComponent"));
	NeonLightComponent->SetupAttachment(MeshComponent);
	NeonLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	NeonLightComponent->SetIntensity(2200.0f);
	NeonLightComponent->SetNeonColor(FLinearColor(0.72f, 0.08f, 1.0f));
}

void ALocker::BeginPlay()
{
	Super::BeginPlay();

	NeonLightComponent->ApplyTo(MeshComponent);
}

FVector ALocker::GetSocketLocation() const
{
	return GetActorLocation();
}

void ALocker::InsertMinion(AMinionLife* Minion)
{
	bClaimed = false;
	bFilled = true;
	StoredMinion = Minion;
	RetrievalSideLocation = Minion->GetFollowTarget()->GetActorLocation();

	Minion->StoreAt(GetSocketLocation());

	if (Door)
	{
		Door->TryOpen();
	}

	if (!bRewardClaimed)
	{
		bRewardClaimed = true;
		Cast<AGMTKPawn>(UGameplayStatics::GetPlayerPawn(this, 0))->GetMinionComponent()->SpawnMinions(
			this,
			RewardMinionCount,
			RewardSpawnRadius,
		RewardSpawnHeight);
	}
}

bool ALocker::RetrieveMinion(AGMTKPawn* Receiver)
{
	if (!StoredMinion
		|| StoredMinion->GetFollowTarget() != Receiver
		|| (Door && !Door->IsOnSameSide(Receiver, RetrievalSideLocation)))
	{
		return false;
	}

	AMinionLife* Minion = StoredMinion;
	StoredMinion = nullptr;
	bFilled = false;
	bClaimed = false;

	Receiver->GetMinionComponent()->RecallStoredMinion(Minion);

	if (Door)
	{
		Door->TryOpen();
	}

	return true;
}

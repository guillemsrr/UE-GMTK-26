// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "Door.h"

#include "Components/StaticMeshComponent.h"
#include "Locker.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	for (ALocker* Locker : Lockers)
	{
		if (Locker)
		{
			Locker->SetDoor(this);
		}
	}

	TryOpen();
}

void ADoor::TryOpen()
{
	if (Lockers.IsEmpty())
	{
		Close();
		return;
	}

	for (const ALocker* Locker : Lockers)
	{
		if (!Locker || !Locker->IsFilled())
		{
			Close();
			return;
		}
	}

	Open();
}

bool ADoor::IsOnSameSide(const AActor* Actor, const FVector& ReferenceLocation) const
{
	const FVector2D ActorDirection(Actor->GetActorLocation() - GetActorLocation());
	const FVector2D ReferenceDirection(ReferenceLocation - GetActorLocation());
	return FVector2D::DotProduct(ActorDirection, ReferenceDirection) >= 0.0f;
}

void ADoor::Open()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ADoor::Close()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

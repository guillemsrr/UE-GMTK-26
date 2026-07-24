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

	if (!Locker)
	{
		return;
	}

	// The locker pushes the result back rather than the door polling it every frame.
	Locker->SetDoor(this);

	if (Locker->IsUnlocked())
	{
		Open();
	}
}

void ADoor::Open()
{
	// Placeholder for the real opening: the hole in the wall is simply no longer there.
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

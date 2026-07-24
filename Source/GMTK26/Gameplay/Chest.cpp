// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "Chest.h"

#include "Components/StaticMeshComponent.h"

#include "Engine/World.h"

#include "Player/GMTKPawn.h"
#include "Player/MinionLife.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void AChest::Open(AActor* Opener)
{
	if (bIsOpen || !MinionClass)
	{
		return;
	}

	bIsOpen = true;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Opener;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 Index = 0; Index < MinionCount; ++Index)
	{
		const FRotator SpawnYaw(0.0f, 360.0f * Index / MinionCount, 0.0f);
		const FVector SpawnLocation = GetActorLocation() + SpawnYaw.RotateVector(FVector::ForwardVector) * SpawnRadius +
		                              FVector(0.0f, 0.0f, SpawnHeight);

		AMinionLife* Minion = GetWorld()->SpawnActor<AMinionLife>(MinionClass,
		                                                          SpawnLocation,
		                                                          SpawnYaw,
		                                                          SpawnParameters);

		if (AGMTKPawn* Pawn = Cast<AGMTKPawn>(Opener))
		{
			Pawn->AddMinion(Minion);
		}
	}
}

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "Chest.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

#include "AI/EnemyPawn.h"
#include "Player/GMTKPawn.h"
#include "Player/MinionComponent.h"
#include "Rendering/GMTKNeonComponent.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	NeonLightComponent = CreateDefaultSubobject<UGMTKNeonComponent>(TEXT("NeonLightComponent"));
	NeonLightComponent->SetupAttachment(MeshComponent);
	NeonLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	NeonLightComponent->SetIntensity(2400.0f);
	NeonLightComponent->SetAttenuationRadius(600.0f);
	NeonLightComponent->SetNeonColor(FLinearColor(1.0f, 0.78f, 0.15f));
}

void AChest::BeginPlay()
{
	Super::BeginPlay();

	NeonLightComponent->ApplyTo(MeshComponent);
}

void AChest::Open(AActor* Opener)
{
	if (bIsOpen)
	{
		return;
	}

	bIsOpen = true;

	Cast<AGMTKPawn>(Opener)->GetMinionComponent()->SpawnMinions(this, MinionCount, SpawnRadius, SpawnHeight);

	SpawnEnemies();
}

void AChest::SpawnEnemies()
{
	if (!EnemyClass || EnemyCount <= 0)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 Index = 0; Index < EnemyCount; ++Index)
	{
		const FRotator SpawnYaw(0.0f, 360.0f * (Index + 0.5f) / EnemyCount, 0.0f);
		const FVector SpawnLocation = GetActorLocation() +
		                              SpawnYaw.RotateVector(FVector::ForwardVector) * EnemySpawnRadius +
		                              FVector(0.0f, 0.0f, EnemySpawnHeight);

		GetWorld()->SpawnActor<AEnemyPawn>(EnemyClass, SpawnLocation, SpawnYaw, SpawnParameters);
	}
}

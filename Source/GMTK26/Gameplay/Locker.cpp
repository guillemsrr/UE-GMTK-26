// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "Locker.h"

#include "Components/StaticMeshComponent.h"
#include "Door.h"
#include "Player/MinionLife.h"

ALocker::ALocker()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ALocker::BeginPlay()
{
	Super::BeginPlay();

	if (!SocketSphereMesh)
	{
		return;
	}

	SocketSpheres.Reserve(RequiredMinions);
	for (int32 Index = 0; Index < RequiredMinions; ++Index)
	{
		UStaticMeshComponent* Sphere = NewObject<UStaticMeshComponent>(this);
		Sphere->SetStaticMesh(SocketSphereMesh);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetRelativeScale3D(FVector(SocketSphereScale));
		Sphere->SetupAttachment(MeshComponent);
		Sphere->RegisterComponent();
		Sphere->SetWorldLocation(GetSocketLocation(Index));

		SocketSpheres.Add(Sphere);
	}
}

int32 ALocker::ClaimSocket()
{
	if (GetFreeSocketCount() <= 0)
	{
		return INDEX_NONE;
	}

	const int32 SocketIndex = InsertedCount + ClaimedCount;
	++ClaimedCount;

	return SocketIndex;
}

void ALocker::ReleaseSocket()
{
	ClaimedCount = FMath::Max(0, ClaimedCount - 1);
}

FVector ALocker::GetSocketLocation(int32 SocketIndex) const
{
	const float Offset = (SocketIndex - (RequiredMinions - 1) * 0.5f) * SocketSpacing;
	return GetActorLocation() + GetActorRightVector() * Offset + FVector(0.0f, 0.0f, SocketHeight);
}

void ALocker::InsertMinion(AMinionLife* Minion, int32 SocketIndex)
{
	if (SocketSpheres.IsValidIndex(SocketIndex) && SocketSpheres[SocketIndex])
	{
		SocketSpheres[SocketIndex]->DestroyComponent();
		SocketSpheres[SocketIndex] = nullptr;
	}

	ClaimedCount = FMath::Max(0, ClaimedCount - 1);
	++InsertedCount;

	Minion->Destroy();

	if (IsUnlocked() && Door)
	{
		Door->Open();
	}
}

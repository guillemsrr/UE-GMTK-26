// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKPawnBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Rendering/GMTKNeonComponent.h"

AGMTKPawnBase::AGMTKPawnBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;

	NeonLightComponent = CreateDefaultSubobject<UGMTKNeonComponent>(TEXT("NeonLightComponent"));
	NeonLightComponent->SetupAttachment(CollisionComponent);
	NeonLightComponent->SetIntensity(2600.0f);
	NeonLightComponent->SetAttenuationRadius(650.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AGMTKPawnBase::BeginPlay()
{
	Super::BeginPlay();

	NeonLightComponent->ApplyTo(MeshComponent);
}

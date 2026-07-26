// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "DormantOrb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/World.h"

#include "Player/GMTKPawn.h"
#include "Player/MinionComponent.h"
#include "Rendering/GMTKNeonComponent.h"
#include "UObject/ConstructorHelpers.h"

ADormantOrb::ADormantOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickupComponent"));
	PickupComponent->SetSphereRadius(120.0f);
	PickupComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(PickupComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(PickupComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetRelativeScale3D(FVector(0.6f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	MeshComponent->SetStaticMesh(SphereMesh.Object);

	NeonLightComponent = CreateDefaultSubobject<UGMTKNeonComponent>(TEXT("NeonLightComponent"));
	NeonLightComponent->SetupAttachment(PickupComponent);
	NeonLightComponent->SetIntensity(1800.0f);
	NeonLightComponent->SetAttenuationRadius(500.0f);
	NeonLightComponent->SetNeonColor(FLinearColor(0.1f, 1.0f, 0.35f));
	NeonLightComponent->SetEmissiveStrength(8.0f);
}

void ADormantOrb::BeginPlay()
{
	Super::BeginPlay();

	NeonLightComponent->ApplyTo(MeshComponent);

	PickupComponent->OnComponentBeginOverlap.AddDynamic(this, &ADormantOrb::OnPickupOverlap);
}

void ADormantOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Bob = FMath::Sin(GetWorld()->GetTimeSeconds() * BobSpeed) * BobHeight;
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, Bob));
}

void ADormantOrb::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent,
                                  AActor* OtherActor,
                                  UPrimitiveComponent* OtherComponent,
                                  int32 OtherBodyIndex,
                                  bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	AGMTKPawn* Pawn = Cast<AGMTKPawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	Pawn->GetMinionComponent()->SpawnMinions(this, MinionCount, 100.0f, 60.0f);

	Destroy();
}

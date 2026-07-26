// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "EnemyPawn.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GMTKPawn.h"
#include "Player/MinionComponent.h"
#include "Player/MinionLife.h"
#include "Rendering/GMTKNeonComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	MinionComponent = CreateDefaultSubobject<UMinionComponent>(TEXT("MinionComponent"));

	NeonLightComponent->SetNeonColor(RestingColor);
	NeonLightComponent->SetEmissiveStrength(7.0f);
	NeonLightComponent->SetIntensity(2400.0f);
	NeonLightComponent->SetAttenuationRadius(600.0f);

	CollisionComponent->SetSphereRadius(70.0f);

	// A cone tipped onto its side is the map's red triangle, and because it now points along the
	// pawn's forward axis it also aims the minion formation at the player.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
	MeshComponent->SetStaticMesh(ConeMesh.Object);
	MeshComponent->SetRelativeLocationAndRotation(FVector(-60.0f, 0.0f, 0.0f), FRotator(-90.0f, 0.0f, 0.0f));
	MeshComponent->SetRelativeScale3D(FVector(1.4f, 1.4f, 1.6f));

	// Hung off the root rather than the mesh, so the wind-up's swell does not drag the number with it.
	MinionCountText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MinionCountText"));
	MinionCountText->SetupAttachment(CollisionComponent);
	MinionCountText->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	MinionCountText->SetHorizontalAlignment(EHTA_Center);
	MinionCountText->SetVerticalAlignment(EVRTA_TextCenter);
	MinionCountText->SetWorldSize(90.0f);
	MinionCountText->SetTextRenderColor(MinionColor.ToFColor(true));
	MinionCountText->SetText(FText::AsNumber(InitialMinionCount));
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	BaseMeshScale = MeshComponent->GetRelativeScale3D();
	StateTime = FMath::Min(WindUpOffset, WindUpTime);

	Player = Cast<AGMTKPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!MinionClass)
	{
		MinionClass = Player->GetMinionClass();
	}

	MinionCountText->SetTextRenderColor(MinionColor.ToFColor(true));

	MinionComponent->Configure(MinionClass, MinionColor);
	MinionComponent->OnMinionCountChanged.AddUObject(this, &AEnemyPawn::HandleMinionCountChanged);
	MinionComponent->OnMinionsDepleted.AddUObject(this, &AEnemyPawn::HandleMinionsDepleted);
	MinionComponent->OnDirectHit.AddUObject(this, &AEnemyPawn::HandleDirectHit);

	// Bound first, so the starting formation writes the number on its way in.
	MinionComponent->SpawnMinions(this, InitialMinionCount, MinionSpawnRadius, MinionSpawnHeight);
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FaceCountTextAtCamera();

	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.0f;
	const float DistanceToPlayer = ToPlayer.Size();

	switch (State)
	{
	case EEnemyState::WindUp:
		{
			// Asleep until the player is in the room: the countdown holds, and so does the tell.
			if (DistanceToPlayer > AggroRadius)
			{
				UpdateTell(0.0f);
				break;
			}

			StateTime += DeltaTime;
			UpdateTell(GetWindUpRatio());

			if (!ToPlayer.IsNearlyZero())
			{
				SetActorRotation(ToPlayer.Rotation());
				MinionComponent->SetFormationYaw(GetActorRotation().Yaw);
			}

			if (StateTime >= WindUpTime)
			{
				if (MinionComponent->DeployAgainst(Player))
				{
					EnterState(EEnemyState::Recovering);
				}
			}
			break;
		}

	case EEnemyState::Recovering:
		{
			StateTime += DeltaTime;
			if (StateTime >= RecoveryTime)
			{
				EnterState(EEnemyState::WindUp);
			}
			break;
		}
	}
}

void AEnemyPawn::Destroyed()
{
	MinionComponent->DestroyMinions();
	Super::Destroyed();
}

void AEnemyPawn::HandleMinionCountChanged(int32 NewCount)
{
	MinionCountText->SetText(FText::AsNumber(NewCount));
}

void AEnemyPawn::FaceCountTextAtCamera()
{
	const APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	// Text is drawn on the component's -X side, so pointing +X away from the camera is what turns the
	// readable face towards it.
	MinionCountText->SetWorldRotation(
		(MinionCountText->GetComponentLocation() - CameraManager->GetCameraLocation()).Rotation());
}

void AEnemyPawn::HandleMinionsDepleted()
{
	Destroy();
}

void AEnemyPawn::HandleDirectHit()
{
	Destroy();
}

void AEnemyPawn::EnterState(EEnemyState NewState)
{
	State = NewState;
	StateTime = 0.0f;

	UpdateTell(0.0f);
}

float AEnemyPawn::GetWindUpRatio() const
{
	return WindUpTime > 0.0f ? FMath::Clamp(StateTime / WindUpTime, 0.0f, 1.0f) : 1.0f;
}

void AEnemyPawn::UpdateTell(float WindUpRatio)
{
	MeshComponent->SetRelativeScale3D(BaseMeshScale * FMath::Lerp(1.0f, WindUpScale, WindUpRatio));

	NeonLightComponent->SetNeonColor(FMath::Lerp(RestingColor, ReadyColor, WindUpRatio));
}

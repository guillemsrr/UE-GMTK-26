// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MinionLife.h"

#include "AIController.h"
#include "AI/EnemyPawn.h"
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "Gameplay/Chest.h"
#include "Gameplay/Locker.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GMTKPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MinionComponent.h"
#include "Rendering/GMTKNeonComponent.h"

namespace
{
	template <typename ActorType, typename PredicateType>
	ActorType* FindNearest(const UWorld* World,
	                       const FVector& Origin,
	                       float& BestDistanceSquared,
	                       PredicateType Predicate)
	{
		ActorType* Nearest = nullptr;
		for (TActorIterator<ActorType> It(World); It; ++It)
		{
			ActorType* Actor = *It;
			if (!Predicate(Actor))
			{
				continue;
			}

			const float DistanceSquared = FVector::DistSquared(Origin, Actor->GetActorLocation());
			if (DistanceSquared < BestDistanceSquared)
			{
				Nearest = Actor;
				BestDistanceSquared = DistanceSquared;
			}
		}

		return Nearest;
	}
}

AMinionLife::AMinionLife()
{
	PrimaryActorTick.bCanEverTick = true;

	NeonLightComponent->SetNeonColor(FLinearColor(0.1f, 1.0f, 0.58f));
	NeonLightComponent->SetEmissiveStrength(8.0f);
	NeonLightComponent->SetIntensity(1200.0f);
	NeonLightComponent->SetAttenuationRadius(360.0f);

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent->SetSphereRadius(24.0f);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MovementComponent->MaxSpeed = 1400.0f;
	MovementComponent->Acceleration = 6000.0f;
	MovementComponent->Deceleration = 6000.0f;
}

void AMinionLife::BeginPlay()
{
	Super::BeginPlay();

	if (!FollowTarget)
	{
		FollowTarget = UGameplayStatics::GetPlayerPawn(this, 0);
	}

	if (FollowTarget)
	{
		FollowMinions = FollowTarget->FindComponentByClass<UMinionComponent>();

		const FVector Offset = GetActorLocation() - FollowTarget->GetActorLocation();
		if (!Offset.IsNearlyZero())
		{
			OrbitAngle = Offset.Rotation().Yaw;
		}
	}
}

void AMinionLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (Mode)
	{
	case EMinionMode::Orbit:
		TickOrbit(DeltaTime);
		break;

	case EMinionMode::Deployed:
		TickDeployed(DeltaTime);
		break;

	case EMinionMode::Spending:
		TickLocker();
		break;

	case EMinionMode::Stored:
		break;
	}
}

void AMinionLife::SetFollowTarget(AActor* NewTarget)
{
	FollowTarget = NewTarget;
	FollowMinions = FollowTarget->FindComponentByClass<UMinionComponent>();
}

void AMinionLife::SetOrbitSlot(int32 SlotIndex, int32 SlotCount)
{
	if (SlotCount <= 1)
	{
		SlotAngle = 0.0f;
		return;
	}

	const float Divisions = OrbitArc >= 360.0f ? SlotCount : SlotCount - 1;
	SlotAngle = OrbitArc * (SlotIndex / Divisions - 0.5f);
}

void AMinionLife::SetNeonColor(const FLinearColor& Color)
{
	NeonLightComponent->SetNeonColor(Color);
}

void AMinionLife::DeployTo(const FVector& Location)
{
	Mode = EMinionMode::Deployed;
	DeployLocation = Location;
	AttackTarget = nullptr;
	WorkScanCooldown = 0.0f;
}

void AMinionLife::DeployAgainst(AActor* Target)
{
	Mode = EMinionMode::Deployed;
	DeployLocation = Target->GetActorLocation();
	AttackTarget = Target;
	WorkScanCooldown = 0.0f;
}

void AMinionLife::Recall()
{
	if (DeployLocker)
	{
		DeployLocker->ReleaseClaim();
		DeployLocker = nullptr;
	}

	AttackTarget = nullptr;
	Mode = EMinionMode::Orbit;
}

void AMinionLife::StoreAt(const FVector& Location)
{
	AttackTarget = nullptr;
	Mode = EMinionMode::Stored;
	MovementComponent->StopMovementImmediately();
	SetActorLocation(Location);
}

void AMinionLife::TickOrbit(float DeltaTime)
{
	if (!FollowTarget)
	{
		return;
	}

	const float AimYaw = FollowMinions->GetFormationYaw();
	const float ToSlot = FRotator::NormalizeAxis(AimYaw + SlotAngle - OrbitAngle);
	OrbitAngle = FRotator::ClampAxis(OrbitAngle + ToSlot * (1.0f - FMath::Exp(-OrbitTurnSpeed * DeltaTime)));

	SteerTo(GetSlotLocation());
}

void AMinionLife::TickDeployed(float DeltaTime)
{
	if (!IsValid(AttackTarget))
	{
		AttackTarget = nullptr;
	}

	if (AttackTarget)
	{
		const FVector TargetLocation = AttackTarget->GetActorLocation();
		SteerTo(TargetLocation);

		if (TryInterceptOrb())
		{
			return;
		}

		const float DirectHitDistance = GetSimpleCollisionRadius() + AttackTarget->GetSimpleCollisionRadius();
		if (FVector::Dist(TargetLocation, GetActorLocation()) <= DirectHitDistance)
		{
			AttackTarget->FindComponentByClass<UMinionComponent>()->ReceiveDirectHit();
			Consume();
		}
		return;
	}

	WorkScanCooldown -= DeltaTime;
	if (WorkScanCooldown <= 0.0f)
	{
		WorkScanCooldown = WorkScanInterval;
		if (FindWork())
		{
			return;
		}
	}

	SteerTo(DeployLocation);
}

bool AMinionLife::TryInterceptOrb()
{
	for (TActorIterator<AMinionLife> It(GetWorld()); It; ++It)
	{
		AMinionLife* Other = *It;
		if (Other == this || Other->Mode == EMinionMode::Stored || Other->Mode == EMinionMode::Spending || Other->
		    FollowTarget == FollowTarget)
		{
			continue;
		}

		const float ContactDistance = GetSimpleCollisionRadius() + Other->GetSimpleCollisionRadius();
		if (FVector::DistSquared(GetActorLocation(), Other->GetActorLocation()) > FMath::Square(ContactDistance))
		{
			continue;
		}

		Other->Consume();
		Consume();
		return true;
	}

	return false;
}

void AMinionLife::Consume()
{
	FollowMinions->RemoveMinion(this);
	Destroy();
}

void AMinionLife::TickLocker()
{
	if (!SteerTo(DeployLocker->GetSocketLocation()))
	{
		return;
	}

	ALocker* Locker = DeployLocker;
	DeployLocker = nullptr;

	Locker->InsertMinion(this);
}

bool AMinionLife::FindWork()
{
	const FVector Origin = GetActorLocation();
	float BestDistanceSquared = FMath::Square(WorkRadius);

	ALocker* Locker = FindNearest<ALocker>(GetWorld(),
	                                       Origin,
	                                       BestDistanceSquared,
	                                       [](const ALocker* Candidate)
	                                       {
		                                       return Candidate->IsAvailable();
	                                       });

	AChest* Chest = FindNearest<AChest>(GetWorld(),
	                                    Origin,
	                                    BestDistanceSquared,
	                                    [](const AChest* Candidate)
	                                    {
		                                    return !Candidate->IsOpen();
	                                    });

	AEnemyPawn* Enemy = FindNearest<AEnemyPawn>(GetWorld(),
	                                            Origin,
	                                            BestDistanceSquared,
	                                            [](const AEnemyPawn* Candidate)
	                                            {
		                                            return Candidate->IsAlive();
	                                            });

	if (Enemy)
	{
		AttackTarget = Enemy;
		return true;
	}

	if (Chest)
	{
		Chest->Open(FollowTarget);
		return true;
	}

	if (Locker)
	{
		Locker->Claim();
		DeployLocker = Locker;
		Mode = EMinionMode::Spending;
		return true;
	}

	return false;
}

FVector AMinionLife::GetSlotLocation() const
{
	const FVector Offset = FRotator(0.0f, OrbitAngle, 0.0f).RotateVector(FVector::ForwardVector) * OrbitRadius;
	return FollowTarget->GetActorLocation() + Offset + FVector(0.0f, 0.0f, OrbitHeight);
}

bool AMinionLife::SteerTo(const FVector& Destination)
{
	const FVector ToDestination = Destination - GetActorLocation();
	const float Distance = ToDestination.Size();
	if (Distance <= ArrivalRadius)
	{
		return true;
	}

	AddMovementInput(ToDestination / Distance, FMath::Min(Distance / BrakingDistance, 1.0f));
	return false;
}

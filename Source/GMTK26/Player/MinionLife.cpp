// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MinionLife.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

AMinionLife::AMinionLife()
{
	PrimaryActorTick.bCanEverTick = true;

	// UFloatingPawnMovement only consumes input while the pawn has a local controller, so an unpossessed minion
	// never moves no matter what AddMovementInput is fed. A plain AIController is enough, and it is what a
	// deployed minion will need anyway once it paths to a target on its own.
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent->SetSphereRadius(24.0f);
	// Pass through the player, enemies and each other: a minion should never shove a pawn around, and contact
	// damage will read these overlaps later.
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Faster than the player, otherwise a trailing minion can never close the gap.
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
		// Seed the orbit from where the minion was placed or spawned so it eases onto its slot instead of
		// swinging around the target first. SetOrbitSlot overrides this once the player owns a set of minions.
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

	if (!FollowTarget)
	{
		return;
	}

	OrbitAngle = FRotator::ClampAxis(OrbitAngle + OrbitAngularSpeed * DeltaTime);

	const FVector ToSlot = GetSlotLocation() - GetActorLocation();
	const float Distance = ToSlot.Size();
	if (Distance <= ArrivalRadius)
	{
		return;
	}

	AddMovementInput(ToSlot / Distance, FMath::Min(Distance / BrakingDistance, 1.0f));
}

FVector AMinionLife::GetSlotLocation() const
{
	const FVector Offset = FRotator(0.0f, OrbitAngle, 0.0f).RotateVector(FVector::ForwardVector) * OrbitRadius;
	return FollowTarget->GetActorLocation() + Offset + FVector(0.0f, 0.0f, OrbitHeight);
}

void AMinionLife::SetFollowTarget(AActor* NewTarget)
{
	FollowTarget = NewTarget;
}

void AMinionLife::SetOrbitSlot(int32 SlotIndex, int32 SlotCount)
{
	OrbitAngle = SlotCount > 0 ? 360.0f * SlotIndex / SlotCount : 0.0f;
}

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MinionLife.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

AMinionLife::AMinionLife()
{
	PrimaryActorTick.bCanEverTick = true;

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

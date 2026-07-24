// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "MinionLife.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Locker.h"
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

	if (DeployLocker)
	{
		if (SteerTo(DeployLocker->GetSocketLocation(DeploySocketIndex)))
		{
			ALocker* Locker = DeployLocker;
			DeployLocker = nullptr;
			Locker->InsertMinion(this, DeploySocketIndex);
		}
		return;
	}

	if (!FollowTarget)
	{
		return;
	}

	OrbitAngle = FRotator::ClampAxis(OrbitAngle + OrbitAngularSpeed * DeltaTime);
	SteerTo(GetSlotLocation());
}

void AMinionLife::SetFollowTarget(AActor* NewTarget)
{
	FollowTarget = NewTarget;
}

void AMinionLife::SetOrbitSlot(int32 SlotIndex, int32 SlotCount)
{
	OrbitAngle = SlotCount > 0 ? 360.0f * SlotIndex / SlotCount : 0.0f;
}

void AMinionLife::DeployToSocket(ALocker* Locker, int32 SocketIndex)
{
	DeployLocker = Locker;
	DeploySocketIndex = SocketIndex;
	FollowTarget = nullptr;
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

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKPlayerCameraManager.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

AGMTKPlayerCameraManager::AGMTKPlayerCameraManager()
{
	CameraActorClass = ACameraActor::StaticClass();
}

void AGMTKPlayerCameraManager::InitializeFor(APlayerController* PC)
{
	Super::InitializeFor(PC);

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = PC;
	SpawnParameters.ObjectFlags |= RF_Transient;
	UseCamera(World->SpawnActor<ACameraActor>(CameraActorClass, FTransform::Identity, SpawnParameters));
}

void AGMTKPlayerCameraManager::UseCamera(ACameraActor* NewCamera)
{
	if (CameraActor == NewCamera)
	{
		return;
	}

	if (CameraActor)
	{
		CameraActor->Destroy();
	}

	CameraActor = NewCamera;

	UCameraComponent* Camera = CameraActor->GetCameraComponent();
	ViewRotation = Camera->GetComponentRotation();
	Camera->SetRelativeRotation(FRotator::ZeroRotator);
}

void AGMTKPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (!CameraActor || !OutVT.Target)
	{
		Super::UpdateViewTargetInternal(OutVT, DeltaTime);
		return;
	}

	const FVector TargetPivot = OutVT.Target->GetActorLocation() + FollowOffset;
	PivotLocation = bHasPivot ? FMath::VInterpTo(PivotLocation, TargetPivot, DeltaTime, FollowSpeed) : TargetPivot;
	bHasPivot = true;

	const FRotationMatrix ViewBasis(FRotator(0.0f, ViewRotation.Yaw, 0.0f));
	const FVector Velocity = OutVT.Target->GetVelocity();
	const FVector2D ScreenVelocity(FVector::DotProduct(Velocity, ViewBasis.GetUnitAxis(EAxis::X)),
	                               FVector::DotProduct(Velocity, ViewBasis.GetUnitAxis(EAxis::Y)));

	const FVector2D TargetTilt = (ScreenVelocity / FMath::Max(TiltReferenceSpeed, 1.0f)).GetClampedToMaxSize(1.0f) *
	                             FVector2D(PitchTilt, RollTilt);
	Tilt = FMath::Vector2DInterpTo(Tilt, TargetTilt, DeltaTime, TiltSpeed);

	const FRotator TiltedRotation(ViewRotation.Pitch + Tilt.X, ViewRotation.Yaw, ViewRotation.Roll + Tilt.Y);
	CameraActor->SetActorLocationAndRotation(PivotLocation - TiltedRotation.Vector() * ViewDistance, TiltedRotation);
	CameraActor->GetCameraComponent()->GetCameraView(DeltaTime, OutVT.POV);
}

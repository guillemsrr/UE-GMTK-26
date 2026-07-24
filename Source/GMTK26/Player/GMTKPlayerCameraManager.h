// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GMTKPlayerCameraManager.generated.h"

class ACameraActor;

UCLASS(Abstract)
class GMTK26_API AGMTKPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AGMTKPlayerCameraManager();

	virtual void InitializeFor(APlayerController* PC) override;

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACameraActor> CameraActorClass;

	UPROPERTY(EditAnywhere)
	float ViewDistance = 900.0f;

	UPROPERTY(EditAnywhere)
	FVector FollowOffset = FVector(0.0f, 0.0f, 60.0f);

	UPROPERTY(EditAnywhere)
	float FollowSpeed = 8.0f;

	UPROPERTY(EditAnywhere)
	float PitchTilt = 2.0f;

	UPROPERTY(EditAnywhere)
	float RollTilt = 3.0f;

	UPROPERTY(EditAnywhere)
	float TiltReferenceSpeed = 900.0f;

	UPROPERTY(EditAnywhere)
	float TiltSpeed = 4.0f;

private:
	void UseCamera(ACameraActor* NewCamera);

	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> CameraActor;

	FVector PivotLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;
	FVector2D Tilt = FVector2D::ZeroVector;
	bool bHasPivot = false;
};

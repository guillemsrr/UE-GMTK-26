// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DormantOrb.generated.h"

class UGMTKNeonComponent;
class UPrimitiveComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class GMTK26_API ADormantOrb : public AActor
{
	GENERATED_BODY()

public:
	ADormantOrb();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PickupComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGMTKNeonComponent> NeonLightComponent;

	UPROPERTY(EditAnywhere)
	int32 MinionCount = 1;

	UPROPERTY(EditAnywhere)
	float BobHeight = 20.0f;

	UPROPERTY(EditAnywhere)
	float BobSpeed = 2.0f;

private:
	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent,
	                     AActor* OtherActor,
	                     UPrimitiveComponent* OtherComponent,
	                     int32 OtherBodyIndex,
	                     bool bFromSweep,
	                     const FHitResult& SweepResult);
};

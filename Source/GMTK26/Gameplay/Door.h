// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ALocker;
class UStaticMeshComponent;

UCLASS(Abstract)
class GMTK26_API ADoor : public AActor
{
	GENERATED_BODY()

protected:
	ADoor();
	virtual void BeginPlay() override;

public:
	void TryOpen();

	bool IsOnSameSide(const AActor* Actor, const FVector& ReferenceLocation) const;

protected:
	void Open();
	void Close();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditInstanceOnly)
	TArray<TObjectPtr<ALocker>> Lockers;
};

// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialTrigger.generated.h"

class AGMTKHUD;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS(Abstract)
class GMTK26_API ATutorialTrigger : public AActor
{
	GENERATED_BODY()

protected:
	ATutorialTrigger();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerComponent;

	UPROPERTY(EditAnywhere)
	TArray<FString> Lines;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComponent,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                         AActor* OtherActor,
	                         UPrimitiveComponent* OtherComponent,
	                         int32 OtherBodyIndex);

	void Show();

	AGMTKHUD* GetGameHUD() const;
};

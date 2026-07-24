// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GMTKPawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
class USpringArmComponent;
class UStaticMeshComponent;
struct FInputActionValue;

UCLASS(Abstract)
class GMTK26_API AGMTKPawn : public APawn
{
	GENERATED_BODY()

public:
	AGMTKPawn();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	virtual void Move(const FInputActionValue& Value);
};

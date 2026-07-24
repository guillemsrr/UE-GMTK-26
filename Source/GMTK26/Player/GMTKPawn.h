// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GMTKPawnBase.h"

#include "GMTKPawn.generated.h"

class ALocker;
class AMinionLife;
class UAbilitySystemComponent;
class UGameplayAbility;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(Abstract)
class GMTK26_API AGMTKPawn : public AGMTKPawnBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	AGMTKPawn();
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddMinion(AMinionLife* Minion);

	// Spends the minion closest to the locker on one of its sockets. False when the player has none
	// to spare or the locker is already fully claimed.
	bool SendMinionToLocker(ALocker* Locker);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractAction;

	virtual void Move(const FInputActionValue& Value);
	virtual void Attack();
	virtual void Interact();

private:
	UPROPERTY()
	TArray<TObjectPtr<AMinionLife>> Minions;

	void RefreshOrbitSlots();
};

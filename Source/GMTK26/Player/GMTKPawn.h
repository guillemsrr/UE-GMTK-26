// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GMTKPawnBase.h"

#include "GMTKPawn.generated.h"

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

	virtual void Move(const FInputActionValue& Value);
	virtual void Attack();

private:
	UPROPERTY()
	TArray<TObjectPtr<AMinionLife>> Minions;
};

// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKPawn.h"

#include "Abilities/GMTKAbility_Attack.h"
#include "Abilities/GMTKAbility_Interact.h"
#include "Gameplay/Locker.h"
#include "AbilitySystemComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "GMTKGameplayTags.h"
#include "InputActionValue.h"
#include "MinionLife.h"

AGMTKPawn::AGMTKPawn()
{
	MovementComponent->MaxSpeed = 900.0f;
	MovementComponent->Acceleration = 4000.0f;
	MovementComponent->Deceleration = 4000.0f;

	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
	MovementComponent->SetPlaneConstraintEnabled(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	DefaultAbilities.Add(UGMTKAbility_Attack::StaticClass());
	DefaultAbilities.Add(UGMTKAbility_Interact::StaticClass());
}

void AGMTKPawn::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->SetPlaneConstraintOrigin(GetActorLocation());

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
	}
}

void AGMTKPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !MappingContext)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AGMTKPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGMTKPawn::Move);
	}

	if (AttackAction)
	{
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &AGMTKPawn::Attack);
	}

	if (InteractAction)
	{
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AGMTKPawn::Interact);
	}
}

UAbilitySystemComponent* AGMTKPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGMTKPawn::AddMinion(AMinionLife* Minion)
{
	Minions.Add(Minion);
	RefreshOrbitSlots();
}

bool AGMTKPawn::SendMinionToLocker(ALocker* Locker)
{
	if (Minions.IsEmpty())
	{
		return false;
	}

	const int32 SocketIndex = Locker->ClaimSocket();
	if (SocketIndex == INDEX_NONE)
	{
		return false;
	}

	const FVector SocketLocation = Locker->GetSocketLocation(SocketIndex);
	int32 NearestIndex = 0;
	float NearestDistanceSquared = TNumericLimits<float>::Max();
	for (int32 Index = 0; Index < Minions.Num(); ++Index)
	{
		const float DistanceSquared = FVector::DistSquared(SocketLocation, Minions[Index]->GetActorLocation());
		if (DistanceSquared < NearestDistanceSquared)
		{
			NearestIndex = Index;
			NearestDistanceSquared = DistanceSquared;
		}
	}

	AMinionLife* Minion = Minions[NearestIndex];
	Minions.RemoveAt(NearestIndex);
	RefreshOrbitSlots();

	Minion->DeployToSocket(Locker, SocketIndex);

	return true;
}

void AGMTKPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();
	if (MoveInput.IsNearlyZero())
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	const FRotator ViewYaw(0.0f, PlayerController->PlayerCameraManager->GetCameraRotation().Yaw, 0.0f);
	const FRotationMatrix ViewBasis(ViewYaw);

	const FVector Direction = ViewBasis.GetUnitAxis(EAxis::X) * MoveInput.Y + ViewBasis.GetUnitAxis(EAxis::Y) *
	                          MoveInput.X;
	AddMovementInput(Direction.GetClampedToMaxSize(1.0f));
}

void AGMTKPawn::Attack()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(GMTKGameplayTags::Ability_Attack.GetTag()));
}

void AGMTKPawn::Interact()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer(GMTKGameplayTags::Ability_Interact.GetTag()));
}

void AGMTKPawn::RefreshOrbitSlots()
{
	const int32 NumberMinions = Minions.Num();
	for (int32 Index = 0; Index < NumberMinions; ++Index)
	{
		Minions[Index]->SetFollowTarget(this);
		Minions[Index]->SetOrbitSlot(Index, NumberMinions);
	}
}

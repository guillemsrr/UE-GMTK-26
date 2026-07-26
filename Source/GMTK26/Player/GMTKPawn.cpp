// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKPawn.h"

#include "Abilities/GMTKAbility_Deploy.h"
#include "Abilities/GMTKAbility_Retrieve.h"
#include "Core/GMTKGameMode.h"
#include "AbilitySystemComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EngineUtils.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Locker.h"
#include "GMTKGameplayTags.h"
#include "InputActionValue.h"
#include "MinionComponent.h"
#include "MinionLife.h"
#include "Rendering/GMTKNeonComponent.h"
#include "TimerManager.h"

#include "Engine/World.h"

AGMTKPawn::AGMTKPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	NeonLightComponent->SetIntensity(3600.0f);
	NeonLightComponent->SetAttenuationRadius(800.0f);

	MovementComponent->MaxSpeed = 900.0f;
	MovementComponent->Acceleration = 4000.0f;
	MovementComponent->Deceleration = 4000.0f;

	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
	MovementComponent->SetPlaneConstraintEnabled(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	MinionComponent = CreateDefaultSubobject<UMinionComponent>(TEXT("MinionComponent"));

	DefaultAbilities.Add(UGMTKAbility_Deploy::StaticClass());
	DefaultAbilities.Add(UGMTKAbility_Retrieve::StaticClass());
}

void AGMTKPawn::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->SetPlaneConstraintOrigin(GetActorLocation());

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	MinionComponent->Configure(MinionClass, FLinearColor(0.1f, 1.0f, 0.58f));
	MinionComponent->OnMinionCountChanged.AddUObject(this, &AGMTKPawn::HandleMinionCountChanged);
	MinionComponent->OnMinionsDepleted.AddUObject(this, &AGMTKPawn::HandleMinionsDepleted);
	MinionComponent->OnDirectHit.AddUObject(this, &AGMTKPawn::HandleDirectHit);

	for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
	}

	//MinionComponent->SpawnMinions(this, 1, 0.0f, 0.0f);
}

void AGMTKPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimYaw();
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

	if (DeployAction)
	{
		EnhancedInput->BindAction(DeployAction, ETriggerEvent::Started, this, &AGMTKPawn::StartDeploying);
		EnhancedInput->BindAction(DeployAction, ETriggerEvent::Completed, this, &AGMTKPawn::StopDeploying);
		EnhancedInput->BindAction(DeployAction, ETriggerEvent::Canceled, this, &AGMTKPawn::StopDeploying);
	}

	if (RetrieveAction)
	{
		EnhancedInput->BindAction(RetrieveAction, ETriggerEvent::Started, this, &AGMTKPawn::Retrieve);
	}
}

UAbilitySystemComponent* AGMTKPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AGMTKPawn::GetMinionCount() const
{
	return MinionComponent->GetMinionCount();
}

bool AGMTKPawn::DeployMinion(const FVector& Location)
{
	return MinionComponent->DeployNearestTo(Location);
}

void AGMTKPawn::RetrieveMinions()
{
	MinionComponent->RecallMinions();

	for (TActorIterator<ALocker> It(GetWorld()); It; ++It)
	{
		It->RetrieveMinion(this);
	}
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

void AGMTKPawn::StartDeploying()
{
	Deploy();

	GetWorldTimerManager().SetTimer(DeployTimer, this, &AGMTKPawn::Deploy, DeployInterval, true);
}

void AGMTKPawn::StopDeploying()
{
	GetWorldTimerManager().ClearTimer(DeployTimer);
}

void AGMTKPawn::Retrieve()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer(GMTKGameplayTags::Ability_Retrieve.GetTag()));
}

void AGMTKPawn::Deploy()
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(GMTKGameplayTags::Ability_Deploy.GetTag()));
}

void AGMTKPawn::HandleMinionCountChanged(int32 MinionCount)
{
	const int32 SpeedRange = FMath::Max(1, MinionsForMaximumMovementSpeed - 1);
	const float MinionRatio = FMath::Clamp(static_cast<float>(MinionCount - 1) / SpeedRange, 0.0f, 1.0f);
	MovementComponent->MaxSpeed = FMath::Lerp(MinimumMovementSpeed, MaximumMovementSpeed, MinionRatio);
}

void AGMTKPawn::HandleMinionsDepleted()
{
	GetWorld()->GetAuthGameMode<AGMTKGameMode>()->EndGame(false);
}

void AGMTKPawn::HandleDirectHit()
{
	GetWorld()->GetAuthGameMode<AGMTKGameMode>()->EndGame(false);
}

void AGMTKPawn::UpdateAimYaw()
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	FHitResult Hit;
	if (!PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		return;
	}

	const FVector ToCursor = (Hit.ImpactPoint - GetActorLocation()) * FVector(1.0f, 1.0f, 0.0f);
	if (ToCursor.IsNearlyZero())
	{
		return;
	}

	AimYaw = ToCursor.Rotation().Yaw;
	MinionComponent->SetFormationYaw(AimYaw);
}

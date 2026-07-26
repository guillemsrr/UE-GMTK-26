// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKGameMode.h"

#include "Components/LightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "EngineUtils.h"
#include "Gameplay/DormantOrb.h"
#include "Gameplay/LevelExit.h"
#include "Gameplay/Locker.h"
#include "Gameplay/TutorialTrigger.h"
#include "GMTKPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/GMTKPawn.h"
#include "Player/GMTKPawnBase.h"
#include "TimerManager.h"
#include "UI/GMTKHUD.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	const TCHAR* RestartOption = TEXT("Restart");
}

AGMTKGameMode::AGMTKGameMode()
{
	DefaultPawnClass = AGMTKPawn::StaticClass();
	PlayerControllerClass = AGMTKPlayerController::StaticClass();
	HUDClass = AGMTKHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> EnvironmentMaterialAsset(
		TEXT("/Game/Visuals/M_MinimalSurface.M_MinimalSurface"));
	EnvironmentMaterial = EnvironmentMaterialAsset.Object;
}

void AGMTKGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bSkipMainMenu = UGameplayStatics::HasOption(Options, RestartOption);
}

void AGMTKGameMode::BeginPlay()
{
	Super::BeginPlay();

	ApplyEnvironmentMaterial();

	if (bSkipMainMenu)
	{
		StartRun();
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(this, &AGMTKGameMode::OpenMainMenu);
}

void AGMTKGameMode::OpenMainMenu()
{
	UGameplayStatics::SetGamePaused(this, true);
	GetGameHUD()->ShowMainMenu();
}

void AGMTKGameMode::ToggleMenu()
{
	AGMTKHUD* GameHUD = GetGameHUD();

	if (!GameHUD->IsMenuOpen())
	{
		PauseRun();
	}
	else if (GameHUD->CanCloseMenu())
	{
		StartRun();
	}
}

void AGMTKGameMode::StartRun()
{
	GetGameHUD()->CloseMenu();
	UGameplayStatics::SetGamePaused(this, false);

	for (TActorIterator<ATutorialTrigger> TriggerIt(GetWorld()); TriggerIt; ++TriggerIt)
	{
		TriggerIt->ShowIfPlayerInside();
	}
}

void AGMTKGameMode::PauseRun()
{
	UGameplayStatics::SetGamePaused(this, true);
	GetGameHUD()->ShowPauseMenu();
}

void AGMTKGameMode::RestartRun()
{
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)), true, RestartOption);
}

void AGMTKGameMode::ReturnToMainMenu()
{
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AGMTKGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AGMTKGameMode::EndGame(bool bPlayerWon)
{
	AGMTKHUD* GameHUD = GetGameHUD();
	if (GameHUD->IsMenuOpen())
	{
		return;
	}

	UGameplayStatics::SetGamePaused(this, true);
	GameHUD->ShowEndScreen(bPlayerWon);
}

AGMTKHUD* AGMTKGameMode::GetGameHUD() const
{
	return GetWorld()->GetFirstPlayerController()->GetHUD<AGMTKHUD>();
}

void AGMTKGameMode::ApplyEnvironmentMaterial()
{
	if (!bUseMinimalEnvironmentMaterial)
	{
		return;
	}

	for (TActorIterator<ADirectionalLight> LightIt(GetWorld()); LightIt; ++LightIt)
	{
		LightIt->GetLightComponent()->SetIntensity(KeyLightIntensity);
	}

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;
		if (Actor->IsA<AGMTKPawnBase>() || Actor->IsA<ALocker>() || Actor->IsA<ADormantOrb>() ||
		    Actor->IsA<ALevelExit>())
		{
			continue;
		}

		TInlineComponentArray<UStaticMeshComponent*> MeshComponents;
		Actor->GetComponents(MeshComponents);
		for (UStaticMeshComponent* MeshComponent : MeshComponents)
		{
			const int32 MaterialCount = MeshComponent->GetNumMaterials();
			for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
			{
				MeshComponent->SetMaterial(MaterialIndex, EnvironmentMaterial);
			}
		}
	}
}

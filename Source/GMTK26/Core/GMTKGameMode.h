// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMTKGameMode.generated.h"

class AGMTKHUD;
class UMaterialInterface;

UCLASS(Abstract)
class GMTK26_API AGMTKGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AGMTKGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

public:
	void ToggleMenu();
	void StartRun();
	void RestartRun();
	void ReturnToMainMenu();
	void QuitGame();

	void EndGame(bool bPlayerWon);

protected:
	UPROPERTY(EditAnywhere)
	bool bUseMinimalEnvironmentMaterial = true;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> EnvironmentMaterial;

	UPROPERTY(EditAnywhere)
	float KeyLightIntensity = 0.75f;

private:
	bool bSkipMainMenu = false;

	void PauseRun();

	void ApplyEnvironmentMaterial();

	void OpenMainMenu();

	AGMTKHUD* GetGameHUD() const;
};

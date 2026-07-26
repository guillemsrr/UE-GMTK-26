// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "TutorialTrigger.h"

#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Player/GMTKPawn.h"
#include "UI/GMTKHUD.h"

#include "Engine/World.h"

ATutorialTrigger::ATutorialTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(TriggerComponent);
}

void ATutorialTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATutorialTrigger::OnTriggerBeginOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ATutorialTrigger::OnTriggerEndOverlap);
}

void ATutorialTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComponent,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (!Cast<AGMTKPawn>(OtherActor))
	{
		return;
	}

	Show();
}

void ATutorialTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComponent,
                                           int32 OtherBodyIndex)
{
	if (!Cast<AGMTKPawn>(OtherActor))
	{
		return;
	}

	GetGameHUD()->HideTutorial(this);
}

void ATutorialTrigger::Show()
{
	GetGameHUD()->ShowTutorial(this, Lines);
}

AGMTKHUD* ATutorialTrigger::GetGameHUD() const
{
	return GetWorld()->GetFirstPlayerController()->GetHUD<AGMTKHUD>();
}

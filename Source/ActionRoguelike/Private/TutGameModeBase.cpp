// Fill out your copyright notice in the Description page of Project Settings.


#include "TutGameModeBase.h"

#include "EngineUtils.h"
#include "TutAttributeComponent.h"
#include "AI/TutAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

ATutGameModeBase::ATutGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ATutGameModeBase::StartPlay()
{
	Super::StartPlay(); //necessary for beginplay() to be triggered on all objects in world

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATutGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void ATutGameModeBase::SpawnBotsTimerElapsed()
{
	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATutGameModeBase::OnBotSpawnQueryCompleted);
}

void ATutGameModeBase::OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}

	// Count the number of alive bots and check against max allowed bots
	int32 NumOfAliveBots = 0;
	for (ATutAICharacter* Bot : TActorRange<ATutAICharacter>(GetWorld()))
	{
		UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(Bot->GetComponentByClass(UTutAttributeComponent::StaticClass()));
		if (ensure(AttributeComp && AttributeComp->IsAlive()))
		{
			NumOfAliveBots++;
		}
	}

	float MaxBotCount = 10.f;

	// Exposed bot count over time curve
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	// Don't spawn bots if we equal or exceed the allowed curve rate
	if (NumOfAliveBots >= MaxBotCount)
	{
		return;
	}

	// Spawn bots
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
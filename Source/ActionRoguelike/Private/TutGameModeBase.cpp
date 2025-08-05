// Fill out your copyright notice in the Description page of Project Settings.


#include "TutGameModeBase.h"

#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TutAttributeComponent.h"
#include "TutCharacter.h"
#include "AI/TutAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tut.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ATutGameModeBase::ATutGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ATutGameModeBase::StartPlay()
{
	Super::StartPlay(); //necessary for beginplay() to be triggered on all objects in world

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATutGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void ATutGameModeBase::KillAll() // @fixme: do over multiple frames to avoid perf spike
{
	for (ATutAICharacter* Bot : TActorRange<ATutAICharacter>(GetWorld()))
	{
		UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp && AttributeComp->IsAlive()))
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void ATutGameModeBase::SpawnBotsTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	// Count the number of alive bots and check against max allowed bots
	int32 NumOfAliveBots = 0;
	for (ATutAICharacter* Bot : TActorRange<ATutAICharacter>(GetWorld()))
	{
		UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Bot);

		if (ensure(AttributeComp))
		{
			if (AttributeComp->IsAlive())
			{
				NumOfAliveBots++;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots);
	
	float MaxBotCount = 10.f;

	// Exposed bot count over time curve
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	// Don't spawn bots if we equal or exceed the allowed curve rate
	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
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
	
	// Spawn bots
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ATutGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ATutCharacter* Player = Cast<ATutCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay; // local in the case of multiple players overriding existing respawn timer

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f; // @fixme: expose this
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ATutGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}
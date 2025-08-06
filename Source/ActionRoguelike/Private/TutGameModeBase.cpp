// Fill out your copyright notice in the Description page of Project Settings.


#include "TutGameModeBase.h"

#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TutAttributeComponent.h"
#include "TutCharacter.h"
#include "TutItemPickup.h"
#include "AI/TutAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tut.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawBotSpawn(TEXT("tut.DebugDrawBotSpawn"), false, TEXT("Enable debug circles for bot spawns."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawPickupSpawn(TEXT("tut.DebugDrawPickupSpawn"), false, TEXT("Enable debug circles for pickup spawns."), ECVF_Cheat);

ATutGameModeBase::ATutGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	KillCreditsAmount = 20.0f;
	SpawnPickupMax = 10.f;
	PlayerStateClass = ATutPlayerState::StaticClass();
}

void ATutGameModeBase::StartPlay()
{
	Super::StartPlay(); //necessary for beginplay() to be triggered on all objects in world

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATutGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
	SpawnRandomPickup();
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
	bool bDebugDraw = CVarDebugDrawBotSpawn.GetValueOnGameThread();
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
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		}
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
}

void ATutGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}



void ATutGameModeBase::SpawnRandomPickup()
{
	// Count the number of spawned pickups and check against max spawned pickups
	int32 NumOfSpawnedPickups = 0;
	for (ATutItemPickup* Pickup : TActorRange<ATutItemPickup>(GetWorld()))
	{
		NumOfSpawnedPickups++;
	}

	int32 RemainingSpawnsAllowed = SpawnPickupMax - NumOfSpawnedPickups;
	
	UE_LOG(LogTemp, Log, TEXT("Found %i spawned Pickups. Can spawn %i more."), NumOfSpawnedPickups, RemainingSpawnsAllowed);

	// Don't spawn pickups if we equal or exceed the allowed max
	if (RemainingSpawnsAllowed <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum pickup capacity. Skipping pickup spawn."));
		return;
	}
	FEnvQueryRequest Request(SpawnPickupQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, FQueryFinishedSignature::CreateUObject(this, &ATutGameModeBase::OnPickupSpawnQueryCompleted, RemainingSpawnsAllowed));
	
}

void ATutGameModeBase::OnPickupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result, int32 NumToSpawn)
{
	FEnvQueryResult* QueryResult = Result.Get();
	bool bDebugDraw = CVarDebugDrawPickupSpawn.GetValueOnGameThread();
	
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn pickup EQS Query failed!"));
		return;
	}

	// Spawn pickups
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	
	if (Locations.Num() > 0 && PickupClasses.Num() > 0)
	{
		for (int32 i = 0; i < NumToSpawn && i < Locations.Num(); i++)
		{
			int32 RandomPickupIndex = FMath::RandRange(0, PickupClasses.Num() - 1);
			TSubclassOf<ATutItemPickup> RandomClass = PickupClasses[RandomPickupIndex];
			int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

			if (RandomClass)
			{
				GetWorld()->SpawnActor<AActor>(RandomClass, Locations[RandomLocationIndex], FRotator::ZeroRotator);
				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Purple, false, 60.0f);
				}

				Locations.RemoveAt(RandomLocationIndex); // remove the location so we don't spawn the same pickup at the same location
			}
		}
	}
}
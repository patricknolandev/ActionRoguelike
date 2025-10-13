// Fill out your copyright notice in the Description page of Project Settings.


#include "TutGameModeBase.h"

#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "TutAttributeComponent.h"
#include "TutCharacter.h"
#include "TutItemPickup.h"
#include "TutPlayerState.h"
#include "TutSaveGame.h"
#include "AI/TutAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tut.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawBotSpawn(TEXT("tut.DebugDrawBotSpawn"), false, TEXT("Enable debug circles for bot spawns."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawPickupSpawn(TEXT("tut.DebugDrawPickupSpawn"), false, TEXT("Enable debug circles for pickup spawns."), ECVF_Cheat);

ATutGameModeBase::ATutGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	KillCreditsAmount = 20;
	SpawnPickupMax = 10;
	RequiredPickupDistance = 300.0f;
	PlayerStateClass = ATutPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ATutGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ATutGameModeBase::StartPlay()
{
	Super::StartPlay(); //necessary for beginplay() to be triggered on all objects in world

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATutGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
	if (ensure(PickupClasses.Num() > 0))
	{
		FEnvQueryRequest Request(SpawnPickupQuery, this);
		Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATutGameModeBase::OnPickupSpawnQueryCompleted);
	}
}

void ATutGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ATutPlayerState* PS = NewPlayer->GetPlayerState<ATutPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ATutPlayerState* PS = KillerPawn->GetPlayerState<ATutPlayerState>();
		if (PS)
		{
			PS->AddCredits(KillCreditsAmount);
		}
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

void ATutGameModeBase::OnPickupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
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

	// For checking distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	while (SpawnCounter < SpawnPickupMax && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove the location so we don't pick the same spawn location again
		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPickupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance check
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup
		int32 RandomPickupIndex = FMath::RandRange(0, PickupClasses.Num() - 1);
		TSubclassOf<ATutItemPickup> RandomPowerupClass = PickupClasses[RandomPickupIndex];
		
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 10, FColor::Purple, false, 60.0f);
		}
		
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void ATutGameModeBase::WriteSaveGame()
{
	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EQS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ATutPlayerState* PS = Cast<ATutPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // single player only at this point
		}
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ATutGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UTutSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data."));
	}
	else
	{
		CurrentSaveGame = Cast<UTutSaveGame>(UGameplayStatics::CreateSaveGameObject(UTutSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."));
	}
}
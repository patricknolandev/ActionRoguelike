// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TutGameModeBase.generated.h"

/**
 * 
 */

class ATutItemPickup;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UTutSaveGame;

UCLASS()
class ACTIONROGUELIKE_API ATutGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;
	
	UPROPERTY()
	UTutSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	TArray<TSubclassOf<ATutItemPickup>> PickupClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	UEnvQuery* SpawnPickupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	float RequiredPickupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	int32 KillCreditsAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	int32 SpawnPickupMax;

	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void SpawnBotsTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	void OnPickupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	ATutGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};

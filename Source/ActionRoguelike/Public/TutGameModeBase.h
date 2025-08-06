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

UCLASS()
class ACTIONROGUELIKE_API ATutGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

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
	float KillCreditsAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	float SpawnPickupMax;

	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void SpawnBotsTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	void OnPickupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result, int32 NumToSpawn);

	UFUNCTION()
	void SpawnRandomPickup();
	
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	ATutGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
};

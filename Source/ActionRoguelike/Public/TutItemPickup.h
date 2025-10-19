// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutItemPickup.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutItemPickup();
	
	void Interact_Implementation(APawn* InstigatorPawn);

	FText GetInteractText_Implementation(APawn* InstigatorPawn);

protected:
	
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	float RespawnTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	float CostCredits;

	FTimerHandle TimerHandle_RespawnTimer;
	
	void ShowPickup();
	void HideAndCooldownPickup();

	UPROPERTY(ReplicatedUsing="OnRep_SetPickupState", BlueprintReadOnly)
	bool bIsActive;
	
	UFUNCTION()
	void OnRep_SetPickupState();
};

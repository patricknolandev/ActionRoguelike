// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutProjectile.h"
#include "TutDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATutDashProjectile : public ATutProjectile
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ATutDashProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	void DashTeleport();

	virtual void Explode_Implementation() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
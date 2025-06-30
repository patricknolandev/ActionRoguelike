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
	FTimerHandle TimerHandle_DashExplosion;

	UFUNCTION(BlueprintCallable)
	void DashHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void PostInitializeComponents() override;

	void DashExplosion();
	
	UPROPERTY(EditAnywhere, Category= "Effects")
	UParticleSystem* DashHitFX;

	FTimerHandle TimerHandle_Teleport;
	FVector TeleportTargetLocation;
	void DashTeleport();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

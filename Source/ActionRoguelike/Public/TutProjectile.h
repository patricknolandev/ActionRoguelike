// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	// Give the attack velocity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	// Make attack visible in world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

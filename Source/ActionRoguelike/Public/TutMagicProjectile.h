// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutMagicProjectile();

protected:

	// Attack collision component
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	// Give the attack velocity
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	// Make attack visible in world
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

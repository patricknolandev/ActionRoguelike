// Fill out your copyright notice in the Description page of Project Settings.


#include "TutDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATutDashProjectile::ATutDashProjectile()
{
	// Speed of projectile
	MovementComp->InitialSpeed = 2000.f;

}

// Called when the game starts or when spawned
void ATutDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



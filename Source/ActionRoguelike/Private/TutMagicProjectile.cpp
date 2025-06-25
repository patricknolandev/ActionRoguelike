// Fill out your copyright notice in the Description page of Project Settings.


#include "TutMagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATutMagicProjectile::ATutMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Speed of projectile
	MovementComp->InitialSpeed = 2000.f;
	
}

// Called when the game starts or when spawned
void ATutMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


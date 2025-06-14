// Fill out your copyright notice in the Description page of Project Settings.


#include "TutMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATutMagicProjectile::ATutMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create collision and set it as root transform component
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
	
	// Create particle component and attach to root
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	
	// Create projectile movement component and set initial speed
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.f;
	
	// Make the projectile visually rotate to follow its velocity direction
	MovementComp->bRotationFollowsVelocity = true;
	// Make the projectile head forward from the player
	MovementComp->bInitialVelocityInLocalSpace = true;
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "TutProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATutProjectile::ATutProjectile()
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

	// Make the projectile visually rotate to follow its velocity direction, and have initial velocity
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ATutProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
}

// Called every frame
void ATutProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TutProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATutProjectile::ATutProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ATutProjectile::OnActorHit);
	RootComponent = SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 8000;

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
}

void ATutProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, Hit.Location, 500.f, 500.f);
	Explode();
}

void ATutProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AudioComp->Play(0); // Play flight sound
	if (GetInstigator()) // Might be nullptr if used by other actors than player
	{
		// Projectile should not collide with actor using it
		SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
		GetInstigator()->MoveIgnoreActorAdd(this);
	}
}

void ATutProjectile::Explode_Implementation()
{
	if (ensure(!IsPendingKillPending()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

// Called when the game starts or when spawned
void ATutProjectile::BeginPlay()
{
	Super::BeginPlay();
}


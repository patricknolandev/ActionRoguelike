// Fill out your copyright notice in the Description page of Project Settings.


#include "TutDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ATutDashProjectile::ATutDashProjectile()
{
	// Speed of projectile
	MovementComp->InitialSpeed = 2000.f;
	MovementComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ATutDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DashExplosion, this, &ATutDashProjectile::DashExplosion, 0.2f);
}

void ATutDashProjectile::DashExplosion()
{
	MovementComp->Velocity = FVector(0, 0, 0);
	EffectComp->SetHiddenInGame(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DashHitFX, GetActorLocation(), GetActorRotation());
	TeleportTargetLocation = GetActorLocation();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATutDashProjectile::DashTeleport, 0.2f);
}

void ATutDashProjectile::DashTeleport()
{
	GetInstigator()->TeleportTo(TeleportTargetLocation,GetInstigator()->GetControlRotation());
	this->Destroy();
}

// Called every frame
void ATutDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATutDashProjectile::DashHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dash Hit %s actor"), *OtherActor->GetName());
	}
	DashExplosion();
}

void ATutDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ATutDashProjectile::DashHit);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "TutDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ATutDashProjectile::ATutDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	
	MovementComp->InitialSpeed = 6000.f;
}

// Called when the game starts or when spawned
void ATutDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ATutDashProjectile::Explode, DetonateDelay);
}

void ATutDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATutDashProjectile::DashTeleport, TeleportDelay);
}


void ATutDashProjectile::DashTeleport()
{
	AActor* ActorToTeleport = GetInstigator();
	FVector TeleportLocation = GetActorLocation();
	if (ensure(ActorToTeleport))
	{
		// If can't teleport using dash proj location (such as shooting landscape)
		// sweep player capsule from above to ground, get first hit on floor as teleport location
		if (!GetInstigator()->TeleportTo(TeleportLocation, ActorToTeleport->GetActorRotation(), false, false))
		{
			FCollisionShape Shape;
			Shape.SetCapsule(34.0f, 88.0f);
			
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
			ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

			FVector StartLocation = TeleportLocation + FVector::UpVector * 500.0f;
			FVector EndLocation = TeleportLocation - FVector::UpVector * 1000.0f;

			FHitResult Hit;
			// May not get a hit
			if (GetWorld()->SweepSingleByObjectType(Hit, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, Shape, QueryParams))
			{
				// Get lush position for teleport
				FVector AdjustedLocation = Hit.ImpactPoint + FVector::UpVector * Shape.GetCapsuleHalfHeight();
				ActorToTeleport->TeleportTo(AdjustedLocation, ActorToTeleport->GetActorRotation(), false, false);
			}
		}
	}
}

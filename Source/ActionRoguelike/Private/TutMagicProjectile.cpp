// Fill out your copyright notice in the Description page of Project Settings.


#include "TutMagicProjectile.h"
#include "TutAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"



// Sets default values
ATutMagicProjectile::ATutMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile can deal damage
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATutMagicProjectile::OnActorOverlap);
	
	// Speed of projectile
	MovementComp->InitialSpeed = 2000.f;
	
}

// Apply damage to hit actors
void ATutMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// Get the attribute component of hit actor
		UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(OtherActor->GetComponentByClass(UTutAttributeComponent::StaticClass()));
		if (AttributeComp) // Hit actor may not have attributes
		{
			// Deal damage and end projectile
			AttributeComp->ApplyHealthChange(-20.0f); 

			Destroy();
		}
	}
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


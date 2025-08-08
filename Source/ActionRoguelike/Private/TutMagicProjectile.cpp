// Fill out your copyright notice in the Description page of Project Settings.


#include "TutMagicProjectile.h"
#include "TutActionComponent.h"
#include "TutGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TutActionEffect.h"



// Sets default values
ATutMagicProjectile::ATutMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile can deal damage
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATutMagicProjectile::OnActorOverlap);
	
	// Speed of projectile
	MovementComp->InitialSpeed = 2000.f;

	DamageAmount = 20.f;
	
}

// Apply damage to hit actors
void ATutMagicProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	 if (OtherActor && OtherActor != GetInstigator())
	 {

	 	UTutActionComponent* ActionComp = OtherActor->FindComponentByClass<UTutActionComponent>();
	 	if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
	 	{
	 		MovementComp->Velocity = -MovementComp->Velocity;

	 		SetInstigator(Cast<APawn>(OtherActor));
	 		return;
	 	}

	 	// Apply damage & impulse
	 	if (UTutGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
	 	{
	 		Explode();

	 		if (ActionComp)
	 		{
	 			ActionComp->AddAction(GetInstigator(), BurningActionClass);
	 		}
	 		
	 	}
	 }
}
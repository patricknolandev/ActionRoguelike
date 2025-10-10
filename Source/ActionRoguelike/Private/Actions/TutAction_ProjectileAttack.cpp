// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutAction_ProjectileAttack.h"

#include "TutAttributeComponent.h"
#include "TutProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UTutAction_ProjectileAttack::UTutAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
	SweepRadius = 20.0f; // size of sphere + distance to offset start of trace
	SweepDistanceFallback = 5000;
	RageCost = 0.0f;
}

void UTutAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	// Check whether we have enough rage, if not, cancel action
	if (ensure(Instigator))
	{
		UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Instigator);
		if (AttributeComp)
		{
			if (AttributeComp->GetRage() < RageCost)
			{
				// Stop action if rage is not enough
				return;
			}
			Super::StartAction_Implementation(Instigator);
		}
	}

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character); //using delegate object to pass character param

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void UTutAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		
		// Always shoot even when clipping with objects on spawn
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(InstigatorCharacter); // ignore the player character in trace
		
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);
		
		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		// Target a point far in the distance as fallback
		FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

		// If we can get a target with a sphere sweep, target the valid object instead
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, Shape, CollisionParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		
		FRotator ProjectileRotation = (TraceEnd - HandLocation).Rotation();
		FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);

		// Deduct rage if the projectile costs rage
		if (UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(InstigatorCharacter))
		{
			if (RageCost > 0.0f)
			{
				AttributeComp->RemoveRage(InstigatorCharacter, RageCost);
			}
		}
		// Shoot projectile at target
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
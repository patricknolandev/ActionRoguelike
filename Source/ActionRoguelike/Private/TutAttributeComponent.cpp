// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAttributeComponent.h"
#include "TutGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tut.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UTutAttributeComponent::UTutAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}

bool UTutAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UTutAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) // for god mode debug
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultipler = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultipler;
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth; // if already dead (0 health), don't trigger apply health change
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ATutGameModeBase* GM = GetWorld()->GetAuthGameMode<ATutGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

bool UTutAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UTutAttributeComponent::IsAtFullHealth() const
{
	return Health >= HealthMax || FMath::IsNearlyEqual(Health, HealthMax, KINDA_SMALL_NUMBER);
}

bool UTutAttributeComponent::IsLowHealth() const
{
	return Health <= HealthMax * 0.25f;
}

float UTutAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

UTutAttributeComponent* UTutAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UTutAttributeComponent>();
	}

	return nullptr;
}

bool UTutAttributeComponent::IsActorAlive(AActor* Actor)
{
	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

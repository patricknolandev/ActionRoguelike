// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAttributeComponent.h"
#include "TutGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tut.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UTutAttributeComponent::UTutAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
	Rage = 0;
	RageMax = 100;
	RageGrantedDmgPercent = 0.25f;

	// SetIsReplicated(true); used for actors outside constructor, ex. setting replication at runtime
	// bReplicates = true; used for actors, not actor components
	SetIsReplicatedByDefault(true); // used for components, "default" is used within constructor
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

	float ActualDelta = Health - OldHealth;

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

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

void UTutAttributeComponent::AddRage(AActor* InstigatorActor, float Delta)
{
	const float NormalizedDelta = FMath::Max(0.0f, FMath::Abs(Delta)); // delta (damage) is negative, make pos valid number for rage
	if (NormalizedDelta <= 0.0f)
	{
		return;
	}

	if (IsAtFullRage())
	{
		return;
	}

	const float OldRage = Rage;
	
	// Give rage based on percentage of damage received
	Rage = FMath::RoundToFloat(FMath::Clamp(Rage + NormalizedDelta * RageGrantedDmgPercent, 0.0f, RageMax));

	const float ActualDelta = Rage - OldRage;
	
	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Rage: %f"), Rage));
	
}

bool UTutAttributeComponent::RemoveRage(AActor* InstigatorActor, float Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Rage < Delta)
	{
		return false;
	}

	// Rage cost delta is positive number here
	
	Rage -= Delta;

	if (Delta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, -Delta);
	}

	return true;
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

bool UTutAttributeComponent::IsAtFullRage() const
{
	return Rage >= RageMax || FMath::IsNearlyEqual(Rage, RageMax, KINDA_SMALL_NUMBER);
}

float UTutAttributeComponent::GetRage() const
{
	return Rage;
}

float UTutAttributeComponent::GetRageMax() const
{
	return RageMax;
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
	UTutAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

void UTutAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewValue,
	float Delta)
{
	// Update clients cosmetic tracking of attributes (UI)
	OnHealthChanged.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void UTutAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void UTutAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTutAttributeComponent, Health);
	DOREPLIFETIME(UTutAttributeComponent, HealthMax);
	//DOREPLIFETIME_CONDITION(UTutAttributeComponent, HealthMax, COND_InitialOnly); // used to optimize for bandwith / cpu
}
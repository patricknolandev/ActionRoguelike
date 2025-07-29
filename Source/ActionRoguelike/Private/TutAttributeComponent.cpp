// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAttributeComponent.h"

// Sets default values for this component's properties
UTutAttributeComponent::UTutAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}

bool UTutAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
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
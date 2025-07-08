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
	Health += Delta;
	Health = FMath::Clamp(Health, 0, HealthMax);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
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
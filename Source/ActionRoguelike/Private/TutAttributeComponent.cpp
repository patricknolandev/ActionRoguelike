// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAttributeComponent.h"

// Sets default values for this component's properties
UTutAttributeComponent::UTutAttributeComponent()
{
	Health = 100;
}


bool UTutAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TutActionComponent.h"

#include "TutAction.h"

UTutActionComponent::UTutActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTutActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UTutAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UTutActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTutActionComponent::AddAction(TSubclassOf<UTutAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTutAction* NewAction = NewObject<UTutAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UTutActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UTutAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UTutActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UTutAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
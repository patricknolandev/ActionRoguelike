// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutActionComponent.h"
#include "Actions/TutAction.h"

UTutActionComponent::UTutActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTutActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UTutAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UTutActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UTutActionComponent::AddAction(AActor* Instigator, TSubclassOf<UTutAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTutAction* NewAction = NewObject<UTutAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UTutActionComponent::RemoveAction(UTutAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
	
}

bool UTutActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UTutAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}
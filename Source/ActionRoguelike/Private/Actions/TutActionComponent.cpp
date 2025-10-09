// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/TutAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UTutActionComponent::UTutActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTutActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UTutAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UTutActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	for (UTutAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(Action->GetOuter()));
		
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UTutActionComponent::AddAction(AActor* Instigator, TSubclassOf<UTutAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTutAction* NewAction = NewObject<UTutAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
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

			// Is client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
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

bool UTutActionComponent::HasActions(TSubclassOf<UTutAction> ActionToCheck)
{
	UClass* ClassToCheck = ActionToCheck.Get();
	if (ClassToCheck)
	{
		for (UTutAction* Action : Actions)
		{
			if (Action->GetClass() == ClassToCheck)
			{
				return true;
			}
		}
	}


	return false;
}

void UTutActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool UTutActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UTutAction* Action : Actions)
	{
		if (Action)
		{
			// Checking if each variable has changed, if so, write it into WroteSomething to be replicated
			// Channel - a "thread" used to replicate the object from server to client
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	// Tells Unreal: "there's a change in this component, please replicate my data"
	return WroteSomething;
}

void UTutActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTutActionComponent, Actions);
}
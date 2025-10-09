// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/TutActionComponent.h"
#include "Net/UnrealNetwork.h"

void UTutAction::Initialize(UTutActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void UTutAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	UTutActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UTutAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	//ensureAlways(bIsRunning);
	
	UTutActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UTutAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	if (ActionComp)
	{
		return ActionComp->GetWorld();
	}
	return nullptr;
}

UTutActionComponent* UTutAction::GetOwningComponent() const
{
	return ActionComp;
}

bool UTutAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	UTutActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UTutAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

bool UTutAction::IsRunning() const
{
	return bIsRunning;
}

void UTutAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTutAction, bIsRunning);
	DOREPLIFETIME(UTutAction, ActionComp);
}
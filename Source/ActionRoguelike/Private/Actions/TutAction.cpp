// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutAction.h"
#include "Actions/TutActionComponent.h"

void UTutAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this));
	
	UTutActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UTutAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	UTutActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UTutAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

UTutActionComponent* UTutAction::GetOwningComponent() const
{
	return Cast<UTutActionComponent>(GetOuter());
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

bool UTutAction::IsRunning() const
{
	return bIsRunning;
}
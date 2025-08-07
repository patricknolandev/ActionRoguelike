// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAction.h"

void UTutAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this));
}

void UTutAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));
}

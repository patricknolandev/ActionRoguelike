// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPlayerController.h"

// This is now covered in UE5 in AController::OnPossessedPawnChanged, is just for example purposes
void ATutPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ATutPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ATutPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

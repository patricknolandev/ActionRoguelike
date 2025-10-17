// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPlayerController.h"

#include "Blueprint/UserWidget.h"

void ATutPlayerController::TogglePauseMenu()
{
	// If we're paused, go back to gameplay and controls
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	// If we're playing, bring up the pause menu and controls
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100); // prevent other UI overlapping our pause menu

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ATutPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ATutPlayerController::TogglePauseMenu);
}

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

//void ATutPlayerController::OnRep_PlayerState()
//{
//	Super::OnRep_PlayerState();

//	OnPlayerStateReceived.Broadcast(PlayerState);
//}

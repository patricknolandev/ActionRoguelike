// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPlayerState.h"

ATutPlayerState::ATutPlayerState()
{
	Credits = 0.0f;
	CreditsMax = 1000000.0f;
}


bool ATutPlayerState::ApplyCreditChange(AActor* InstigatorActor, float Delta)
{

	float OldCredits = Credits;

	Credits = FMath::Clamp(Credits + Delta, 0.0f, CreditsMax);

	float ActualDelta = Credits - OldCredits;
	OnCreditsChanged.Broadcast(InstigatorActor, this, Credits, ActualDelta);

	// Not enough credits for transaction
	if (ActualDelta < 0.0f && Credits == 0.0f)
	{
		// TO-DO
	}
	return ActualDelta != 0.0f;
}

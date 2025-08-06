// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPlayerState.h"

ATutPlayerState::ATutPlayerState()
{
	Credits = 0.0f;
	CreditsMax = 1000000.0f;
}


ATutPlayerState* ATutPlayerState::GetPlayerState(AActor* FromActor)
{
	if (FromActor)
	{
		APawn* MyPawn = Cast<APawn>(FromActor);
		if (MyPawn)
		{
			return MyPawn->GetPlayerState<ATutPlayerState>();
		}
		AController* MyController = Cast<AController>(FromActor);
		if (MyController)
		{
			return MyController->GetPlayerState<ATutPlayerState>();
		}
	}
	return nullptr;
}

bool ATutPlayerState::ApplyCreditChange(AActor* InstigatorActor, float Delta)
{

	float OldCredits = Credits;

	Credits = FMath::Clamp(Credits + Delta, 0.0f, CreditsMax);

	float ActualDelta = Credits - OldCredits;
	OnCreditsChanged.Broadcast(InstigatorActor, this, Credits, ActualDelta);
	
	return ActualDelta != 0.0f;
}

bool ATutPlayerState::HasEnoughCredits(float Delta) const
{
	return Credits >= Delta;
}

bool ATutPlayerState::IsAtFullCredits() const
{
	return Credits >= CreditsMax || FMath::IsNearlyEqual(Credits, CreditsMax, KINDA_SMALL_NUMBER);
}
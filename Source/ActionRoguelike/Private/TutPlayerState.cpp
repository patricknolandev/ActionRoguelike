// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPlayerState.h"

ATutPlayerState::ATutPlayerState()
{
	Credits = 0;
	CreditsMax = 1000000;
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

void ATutPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ATutPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

int32 ATutPlayerState::GetCredits() const
{
	return Credits;
}

bool ATutPlayerState::IsAtFullCredits() const
{
	return Credits >= CreditsMax || FMath::IsNearlyEqual(Credits, CreditsMax, KINDA_SMALL_NUMBER);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupCoin.h"
#include "TutPlayerState.h"

ATutPickupCoin::ATutPickupCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreditAmount = 5.0f;
}

void ATutPickupCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	ATutPlayerState* PS = ATutPlayerState::GetPlayerState(InstigatorPawn);
	if (ensure(PS) && !PS->IsAtFullCredits())
	{
		if (PS->ApplyCreditChange(this, CreditAmount))
		{
			HideAndCooldownPickup();
		}
	}
}

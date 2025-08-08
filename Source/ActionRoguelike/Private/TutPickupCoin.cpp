// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupCoin.h"
#include "TutPlayerState.h"

ATutPickupCoin::ATutPickupCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreditAmount = 5;
}

void ATutPickupCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ATutPlayerState* PS = ATutPlayerState::GetPlayerState(InstigatorPawn))
	{
		PS->AddCredits(CreditAmount);
		HideAndCooldownPickup();
	}
}

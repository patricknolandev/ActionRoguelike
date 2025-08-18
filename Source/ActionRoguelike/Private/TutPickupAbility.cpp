// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupAbility.h"
#include "Actions/TutAction.h"
#include "Actions/TutActionComponent.h"

ATutPickupAbility::ATutPickupAbility()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATutPickupAbility::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn) && Ability != nullptr)
	{
		UTutActionComponent* ActionComp = InstigatorPawn->FindComponentByClass<UTutActionComponent>();
		if (ensure(ActionComp) && !ActionComp->HasActions(Ability))
		{
			ActionComp->AddAction(InstigatorPawn, Ability);
			HideAndCooldownPickup();
		}
	}
}

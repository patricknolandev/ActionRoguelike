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
		// Check if player already has Ability
		if (ActionComp)
		{
			if (ActionComp->HasActions(Ability))
			{
				FString DebugMsg = FString::Printf(TEXT("Action '%s' already known by %s."), *GetNameSafe(Ability), *GetNameSafe(InstigatorPawn));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
				return;
			}

			// Give new Ability to player
			ActionComp->AddAction(InstigatorPawn, Ability);
			HideAndCooldownPickup();
		}
	}
}

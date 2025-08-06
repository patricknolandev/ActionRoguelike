// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupHealthPotion.h"
#include "TutAttributeComponent.h"
#include "TutPlayerState.h"

ATutPickupHealthPotion::ATutPickupHealthPotion()
{
	// Don't want interact to pick up the mesh collision
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealAmount = 100.0f;
	CostCredits = 5.0f;
}

void ATutPickupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp)
	{
		if (ensure(AttributeComp && !AttributeComp->IsAtFullHealth()))
		{
			ATutPlayerState* PS = ATutPlayerState::GetPlayerState(InstigatorPawn);
			if (ensure(PS && PS->HasEnoughCredits(CostCredits)))
			{
				// Only activate on successful heal and credit deduction
				if (AttributeComp->ApplyHealthChange(this, HealAmount) && PS->ApplyCreditChange(this, -CostCredits))
				{
					HideAndCooldownPickup();
				}
			}
		}
	}
}

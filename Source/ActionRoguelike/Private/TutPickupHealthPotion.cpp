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
	CostCredits = 5;
}

void ATutPickupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(InstigatorPawn);
	if (ensure(AttributeComp))
	{
		if (!AttributeComp->IsAtFullHealth())
		{
			if (ATutPlayerState* PS = ATutPlayerState::GetPlayerState(InstigatorPawn))
			{
				if (PS->RemoveCredits(CostCredits) && AttributeComp->ApplyHealthChange(this, HealAmount))
				{
					Super::Interact_Implementation(InstigatorPawn);
				}
			}
		}
	}
}

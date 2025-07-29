// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupHealthPotion.h"
#include "TutAttributeComponent.h"

ATutPickupHealthPotion::ATutPickupHealthPotion()
{
	// Don't want interact to pick up the mesh collision
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealAmount = 100.0f;
}

void ATutPickupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(InstigatorPawn->GetComponentByClass(UTutAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		// Check if not at max health
		if (ensure(AttributeComp && !AttributeComp->IsAtFullHealth()))
		{
			// Only activate on successful heal
			if (AttributeComp->ApplyHealthChange(HealAmount))
			{
				HideAndCooldownPickup();
			}
		}
	}
}

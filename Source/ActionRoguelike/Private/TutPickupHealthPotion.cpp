// Fill out your copyright notice in the Description page of Project Settings.


#include "TutPickupHealthPotion.h"
#include "TutAttributeComponent.h"
#include "Components/SphereComponent.h"

ATutPickupHealthPotion::ATutPickupHealthPotion()
{
	HealAmount = 50.0f;
	RespawnTime = 10.0f;
	// Don't want interact to pick up the mesh collision
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATutPickupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(InstigatorPawn->GetComponentByClass(UTutAttributeComponent::StaticClass())))
	{
		if (AttributeComp->IsAtFullHealth())
		{
			return;
		}

		AttributeComp->ApplyHealthChange(HealAmount);
		
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetVisibility(false, true);
		GetWorldTimerManager().SetTimer(TimerHandle_PotionRespawn, this, &ATutPickupHealthPotion::Respawn, RespawnTime);
	}
}

void ATutPickupHealthPotion::Respawn()
{
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetVisibility(true, true);
}
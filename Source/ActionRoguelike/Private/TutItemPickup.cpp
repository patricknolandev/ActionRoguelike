// Fill out your copyright notice in the Description page of Project Settings.


#include "TutItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ATutItemPickup::ATutItemPickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
	CostCredits = 0.0f;

	bReplicates = true;
}

void ATutItemPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes
}

void ATutItemPickup::ShowPickup()
{
	SetPickupState(true);
}

void ATutItemPickup::HideAndCooldownPickup()
{
	SetPickupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ATutItemPickup::ShowPickup, RespawnTime);
}

void ATutItemPickup::SetPickupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive, true);
}


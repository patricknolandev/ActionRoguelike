// Fill out your copyright notice in the Description page of Project Settings.


#include "TutItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATutItemPickup::ATutItemPickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
	CostCredits = 0.0f;

	bReplicates = true;
	bIsActive = true;
}

void ATutItemPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes

	// Hide pickup
	bIsActive = false;
	OnRep_SetPickupState();
	// Respawn after a delay
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ATutItemPickup::ShowPickup, RespawnTime);
}

void ATutItemPickup::ShowPickup()
{
	bIsActive = true;
	OnRep_SetPickupState();
}

void ATutItemPickup::OnRep_SetPickupState()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ATutItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATutItemPickup, bIsActive);
}

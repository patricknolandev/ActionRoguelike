// Fill out your copyright notice in the Description page of Project Settings.


#include "TutItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATutItemChest::ATutItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;

	// Replicate this with server and client (pre-init using bReplicates, SetReplicates() is for runtime)
	bReplicates = true;
	
}

void ATutItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	// Open lid visually on server manually
	OnRep_LidOpened();
}

void ATutItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// When this property changes, send it to all clients
	DOREPLIFETIME(ATutItemChest, bLidOpened);
}

void ATutItemChest::OnRep_LidOpened()
{
	// Open / close lid visually using repnotify
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TutItemChest.h"
#include "Components/StaticMeshComponent.h"

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
	
}

void ATutItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Called when the game starts or when spawned
void ATutItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


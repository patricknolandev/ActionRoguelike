// Fill out your copyright notice in the Description page of Project Settings.

#include "TutCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATutCharacter::ATutCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATutCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

// Called every frame
void ATutCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATutCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}


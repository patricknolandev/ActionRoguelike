// Fill out your copyright notice in the Description page of Project Settings.

#include "TutCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATutCharacter::ATutCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach a spring arm component to the character
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	// Attach a camera component to the spring arm so its controlled by it
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// Get the character to rotate towards the direction of acceleration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Disable the character rotating towards the yaw of the player controller 
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ATutCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	// Set up the inputs for moving on XY axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ATutCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATutCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATutCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATutCharacter::Jump);
}

void ATutCharacter::MoveForward(float Value) // Player should move forward in direction of camera
{
	// Store the controller rotation
	FRotator ControlRot = GetControlRotation();
	// Only get the yaw so we don't move up/down/roll based on camera
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	// Move in the horizontal direction of the camera
	AddMovementInput(ControlRot.Vector(), Value);
}

void ATutCharacter::MoveRight(float Value) // Player should move left and right based on camera yaw
{
	// Store player controller rotation and only get yaw
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X - Forward (Red)
	// Y - Right (Green)
	// Z - Up (Blue)

	// Store the Y rotation of the player controller
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	// Move right & left based on controller Y rotation
	AddMovementInput(RightVector, Value);
}

// Shoot magic projectile from hand
void ATutCharacter::PrimaryAttack()
{
	// Location to spawn, relative to controller rotation
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	// Always shoot even when clipping with objects on spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Shoot projectile
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ATutCharacter::Jump()
{
	Super::Jump();
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "TutCharacter.h"

#include "Actions/TutActionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TutInteractionComponent.h"
#include "TutAttributeComponent.h"
#include "Components/CapsuleComponent.h"

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
	
	InteractionComp = CreateDefaultSubobject<UTutInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UTutAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UTutActionComponent>("ActionComp");
	
	// Get the character to rotate towards the direction of acceleration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Disable the character rotating towards the yaw of the player controller 
	bUseControllerRotationYaw = false;
	
	TimeToHitParamName = "TimeToHit";
}

void ATutCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ATutCharacter::OnHealthChanged);
}

FVector ATutCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation(); // overrides eye location used in interaction trace
}

// Called to bind functionality to input - how we control the player character
void ATutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up the inputs for moving on XY axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ATutCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATutCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATutCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ATutCharacter::AbilityBlackhole);
	PlayerInputComponent->BindAction("Mobility", IE_Pressed, this, &ATutCharacter::MobilityDash);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATutCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ATutCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATutCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATutCharacter::SprintStop);
}

void ATutCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
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

void ATutCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ATutCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

// Shoot magic projectile from hand
void ATutCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ATutCharacter::AbilityBlackhole()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void ATutCharacter::MobilityDash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ATutCharacter::Jump()
{
	Super::Jump();
}

void ATutCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ATutCharacter::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	// Taking damage
	if (NewHealth > 0 && Delta < 0.0f)
	{
		// Hit-flash character material
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		// Give rage based on % of damage
		if (OwningComp->GetRage() < OwningComp->GetRageMax())
		{
			OwningComp->AddRage(InstigatorActor, Delta);
		}
	}
	// Player dies
	if (NewHealth <= 0 && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Remove corpse after time
		SetLifeSpan(5.0f);
	}
}



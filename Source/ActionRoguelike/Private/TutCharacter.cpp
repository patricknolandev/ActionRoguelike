// Fill out your copyright notice in the Description page of Project Settings.

#include "TutCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TutInteractionComponent.h"

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

void ATutCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	// Trace from camera to get projectile target destination
	FHitResult Hit;
	int Range = 100000;
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector End = CameraLocation + (CameraComp->GetForwardVector() * Range);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // ignore the player character in trace
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectParams, CollisionParams);
	FVector ImpactPoint;
	if (bBlockingHit) // set target to line trace hit location or to end of trace
	{
		ImpactPoint = Hit.Location;
	}
	else
	{
		ImpactPoint = End;	
	}

	// Store the hand spawn location and target trace location
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, ImpactPoint);
	FTransform SpawnTM = FTransform(TargetRot, HandLocation);
	
	// Always shoot even when clipping with objects on spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	// Shoot projectile
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	//DrawDebugLine(GetWorld(), HandLocation, ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0f);

}

// Shoot magic projectile from hand
void ATutCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATutCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	//	GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}
void ATutCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ATutCharacter::AbilityBlackhole()
{
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATutCharacter::AbilityBlackhole_TimeElapsed, 0.2f);
}

void ATutCharacter::AbilityBlackhole_TimeElapsed()
{
	SpawnProjectile(AbilityClass);
}

void ATutCharacter::MobilityDash()
{
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATutCharacter::MobilityDash_TimeElapsed, 0.2f);
}

void ATutCharacter::MobilityDash_TimeElapsed()
{
	SpawnProjectile(MobilityClass);
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

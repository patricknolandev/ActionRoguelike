// Fill out your copyright notice in the Description page of Project Settings.

#include "TutCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TutInteractionComponent.h"
#include "TutAttributeComponent.h"
#include "TutProjectile.h"
#include "Kismet/GameplayStatics.h"

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
	
	// Get the character to rotate towards the direction of acceleration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Disable the character rotating towards the yaw of the player controller 
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";
}

void ATutCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ATutCharacter::OnHealthChanged);
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

void ATutCharacter::SpawnProjectile(TSubclassOf<ATutProjectile> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		SweepRadius = 20.0f;
		SweepDistanceFallback = 5000;
		
		// Always shoot even when clipping with objects on spawn
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = GetInstigator();

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);
		
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // ignore the player character in trace
		
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceDirection = GetInstigator()->GetControlRotation().Vector();
		FVector TraceStart = GetInstigator()->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		// Target a point far in the distance as fallback
		FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

		// If we can get a target with a sphere sweep, target the valid object instead
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, Shape, CollisionParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		
		FRotator ProjectileRotation = (TraceEnd - HandLocation).Rotation();
		FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);
		
		// Shoot projectile at target
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

// Shoot magic projectile from hand
void ATutCharacter::PrimaryAttack()
{
	StartAttackEffects();
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATutCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ATutCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
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

void ATutCharacter::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	// Taking damage
	if (NewHealth > 0 && Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	}
	// Player dies
	if (NewHealth <= 0 && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}



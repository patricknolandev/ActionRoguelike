// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TutAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Actions/TutActionComponent.h"
#include "TutAttributeComponent.h"
#include "TutPlayerState.h"
#include "TutWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATutAICharacter::ATutAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UTutAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UTutActionComponent>("ActionComp");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // Make sure the AI always uses the controller so it will run behavior

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true); // necessary for projectiles to add impulses to ragdoll enemies
	
	TimeToHitParamName = "TimeToHit";

}


void ATutAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATutAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATutAICharacter::OnHealthChanged);

}

void ATutAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Only trigger on new targets
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		MulticastSpawnSpottedWidget();
	}
}

void ATutAICharacter::MulticastSpawnSpottedWidget_Implementation()
{
	// Spawn spotted widget for server and clients
	if (PlayerSpottedWidget == nullptr)
	{
		PlayerSpottedWidget = CreateWidget<UTutWorldUserWidget>(GetWorld(), PlayerSpottedWidgetClass);
		if (PlayerSpottedWidget)
		{
			PlayerSpottedWidget->AttachedActor = this;
			// Place on top of other widget layers as prio
			PlayerSpottedWidget->AddToViewport(10);
		}
	}
}

AActor* ATutAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}
	return nullptr;
}

void ATutAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ATutAICharacter::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f) 	// If damage is taken
	{

		if (InstigatorActor != this && !InstigatorActor->IsA(ATutAICharacter::StaticClass())) // don't target ourselves or allies
		{
			SetTargetActor(InstigatorActor); // Focus the player who hit us
		}

		if (ActiveHealthBar == nullptr) // if we don't already have a healthbar showing
		{
			ActiveHealthBar = CreateWidget<UTutWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		
		if (NewHealth <= 0.0f) // If AI just died
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// ragdoll and set correct collision
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// set lifespan (how long before we destroy actor)
			SetLifeSpan(10.0f);
		}
	}
}
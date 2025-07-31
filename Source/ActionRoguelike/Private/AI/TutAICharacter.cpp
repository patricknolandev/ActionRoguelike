// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TutAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TutAttributeComponent.h"
#include "AI/TutAIController.h"
#include "Components/CapsuleComponent.h"

ATutAICharacter::ATutAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UTutAttributeComponent>("AttributeComp");
}


void ATutAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATutAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATutAICharacter::OnHealthChanged);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // Make sure the AI always uses the controller so it will run behavior
}

void ATutAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
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
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// set lifespan (how long before we destroy actor)
			SetLifeSpan(10.0f);
		}
	}
}
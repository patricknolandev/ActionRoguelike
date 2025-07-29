// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TutBTService_CheckLowHealth.h"

#include "AIController.h"
#include "TutAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UTutBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Check if AI pawn is low health every 0.5s (time interval defined in parent function)
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if ensure(BlackboardComp)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(AIPawn->GetComponentByClass(UTutAttributeComponent::StaticClass()));
				if (ensure(AttributeComp))
				{
					bool bIsLowhealth = false;
					if (AttributeComp->IsLowHealth())
					{
						bIsLowhealth = true;
					}
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowhealth);
				}
			}
		}
	}
}

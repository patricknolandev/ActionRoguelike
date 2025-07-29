// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TutBTTask_HealSelf.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "TutAttributeComponent.h"

EBTNodeResult::Type UTutBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
			UTutAttributeComponent* AttributeComp = Cast<UTutAttributeComponent>(MyPawn->GetComponentByClass(UTutAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				bool bHealed = AttributeComp->ApplyHealthChange(HealAmount);
				return bHealed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
	}
	return EBTNodeResult::Failed;
}

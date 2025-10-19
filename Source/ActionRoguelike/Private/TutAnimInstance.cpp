// Fill out your copyright notice in the Description page of Project Settings.


#include "TutAnimInstance.h"
#include "GameplayTagContainer.h"
#include "Actions/TutActionComponent.h"

void UTutAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UTutActionComponent>(OwningActor->GetComponentByClass(UTutActionComponent::StaticClass()));
	}
}

void UTutAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}

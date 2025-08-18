// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutActionEffect_Thorns.h"
#include "TutAttributeComponent.h"
#include "TutGameplayFunctionLibrary.h"
#include "Actions/TutActionComponent.h"

UTutActionEffect_Thorns::UTutActionEffect_Thorns()
{
	Duration = 0.0f;
	ReflectionMultiplier = 0.25f;
}


void UTutActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	if (Delta < 0.0f) 	// If damage is taken (ignore healing)
	{
		UTutActionComponent* OwnerComp = GetOwningComponent();
		if (AActor* OwnerActor = OwnerComp->GetOwner())
		{
			if (InstigatorActor != OwnerActor) // don't reflect at ourselves
			{
				if (UTutActionComponent* InstigatorComp = Cast<UTutActionComponent>(InstigatorActor->GetComponentByClass(UTutActionComponent::StaticClass())))
				{
					FGameplayTag ReflectDamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.NoReflect"));

					// If damage has already been reflected once, do not reflect again
					if (InstigatorComp->ActiveGameplayTags.HasTag(ReflectDamageTag))
					{
						return;
					}
					OwnerComp->ActiveGameplayTags.AddTag(ReflectDamageTag);

					// Always reflect min 1 damage after reflect multi
					float ReflectDamageAmount = FMath::Max(1.0f, FMath::RoundToFloat(FMath::Abs(Delta) * ReflectionMultiplier));
					UTutGameplayFunctionLibrary::ApplyDamage(OwnerActor, InstigatorActor, ReflectDamageAmount);

					OwnerComp->ActiveGameplayTags.RemoveTag(ReflectDamageTag);
				}
			}
		}
	}
}


void UTutActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Instigator);

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UTutActionEffect_Thorns::OnHealthChanged);
	}

}

void UTutActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(Instigator);

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UTutActionEffect_Thorns::OnHealthChanged);
	}
}

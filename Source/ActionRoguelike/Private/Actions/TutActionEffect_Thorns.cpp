// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutActionEffect_Thorns.h"
#include "TutAttributeComponent.h"
#include "TutGameplayFunctionLibrary.h"
#include "Actions/TutActionComponent.h"

UTutActionEffect_Thorns::UTutActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;
	ReflectionMultiplier = 0.25f;
}

void UTutActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Thorns StartAction called! Instigator: %s"), *GetNameSafe(Instigator));
	
	Super::StartAction_Implementation(Instigator);

	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UTutActionEffect_Thorns::OnHealthChanged);
	}

}

void UTutActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UTutAttributeComponent* AttributeComp = UTutAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UTutActionEffect_Thorns::OnHealthChanged);
	}
}

void UTutActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	UTutActionComponent* OwnerComp = GetOwningComponent();
	if (AActor* OwnerActor = OwnerComp->GetOwner())
	{
		// If damage is taken (ignore healing), don't reflect at ourselves
		if (Delta < 0.0f && InstigatorActor != OwnerActor) 	
		{
			if (UTutActionComponent* InstigatorComp = Cast<UTutActionComponent>(InstigatorActor->GetComponentByClass(UTutActionComponent::StaticClass())))
			{
				FGameplayTag ReflectDamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.NoReflect"));

				// If damage has already been reflected once, do not reflect again
				if (InstigatorComp->ActiveGameplayTags.HasTag(ReflectDamageTag))
				{
					return;
				}

				// Prevent reflect pingpong
				OwnerComp->ActiveGameplayTags.AddTag(ReflectDamageTag); 

				// Always reflect min 1 damage after reflect multi
				int ReflectDamageAmount = FMath::Max(1.0f, FMath::RoundToInt(FMath::Abs(Delta) * ReflectionMultiplier));
				UTutGameplayFunctionLibrary::ApplyDamage(OwnerActor, InstigatorActor, ReflectDamageAmount);

				OwnerComp->ActiveGameplayTags.RemoveTag(ReflectDamageTag);
			}
		}
	}
}
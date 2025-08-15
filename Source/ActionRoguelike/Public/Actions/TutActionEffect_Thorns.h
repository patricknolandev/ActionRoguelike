// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutAttributeComponent.h"
#include "Actions/TutActionEffect.h"
#include "TutActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTutActionEffect_Thorns : public UTutActionEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float ReflectionMultiplier;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:

	UTutActionEffect_Thorns();


	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutGameplayInterface.h"
#include "TutItemPickup.h"
#include "TutPickupHealthPotion.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutPickupHealthPotion : public ATutItemPickup, public ITutGameplayInterface
{
	GENERATED_BODY()

public:

	ATutPickupHealthPotion();
	
	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	float HealAmount;
	
};

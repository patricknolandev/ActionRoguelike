// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutGameplayInterface.h"
#include "TutItemPickup.h"
#include "TutPickupAbility.generated.h"

class UTutAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATutPickupAbility : public ATutItemPickup, public ITutGameplayInterface
{
	GENERATED_BODY()


public:

	ATutPickupAbility();

	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay")
	TSubclassOf<UTutAction> Ability;
};

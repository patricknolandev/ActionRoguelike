// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutGameplayInterface.h"
#include "TutItemPickup.h"
#include "TutPickupCoin.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutPickupCoin : public ATutItemPickup, public ITutGameplayInterface
{
	GENERATED_BODY()

public:

	ATutPickupCoin();
	
	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	float CreditAmount;
	
};

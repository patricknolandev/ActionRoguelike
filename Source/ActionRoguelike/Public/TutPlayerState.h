// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TutPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditsChanged, AActor*, InstigatorActor, ATutPlayerState*, OwningPlayerState, float, NewCredits, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATutPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ATutPlayerState();

protected:

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	float Credits;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	float CreditsMax;

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyCreditChange(AActor* InstigatorActor, float Delta);

	bool HasEnoughCredits(float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged; // use this in BP to update HUD
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TutAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ATutAICharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
};

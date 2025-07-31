// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TutAICharacter.generated.h"

class UPawnSensingComponent;
class UTutAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ATutAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	void SetTargetActor(AActor* NewTarget);
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTutAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TutAICharacter.generated.h"

class UTutActionComponent;
class UPawnSensingComponent;
class UTutAttributeComponent;
class UUserWidget;
class UTutWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ATutAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ATutAICharacter();

protected:
	
	UTutWorldUserWidget* ActiveHealthBar;

	UTutWorldUserWidget* PlayerSpottedWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSpottedWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	void SetTargetActor(AActor* NewTarget);
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTutAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTutActionComponent* ActionComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnSpottedWidget();
	
};

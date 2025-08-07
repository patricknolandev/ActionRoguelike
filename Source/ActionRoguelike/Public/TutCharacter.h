// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutPlayerState.h"
#include "TutProjectile.h"
#include "GameFramework/Character.h"
#include "TutCharacter.generated.h"

class UTutActionComponent;
class UCameraComponent;
class USpringArmComponent;
class UTutInteractionComponent;
class UAnimMontage;
class UTutAttributeComponent;
class ATutProjectile;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ATutCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UTutInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTutAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTutActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();
	
	void PrimaryAttack();
	void AbilityBlackhole();
	void MobilityDash();
	
	virtual void Jump() override;
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:
	
	ATutCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};

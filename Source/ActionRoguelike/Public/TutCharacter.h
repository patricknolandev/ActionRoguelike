// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "TutCharacter.generated.h"

class UInputMappingContext;
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_LookMouse;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_LookStick;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Sprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Mobility;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Ability;

	void Move(const FInputActionInstance& Instance);
	/*void MoveForward(float Value);
	void MoveRight(float Value);*/

	void SprintStart();
	void SprintStop();
	void LookMouse(const FInputActionValue& InputValue);
	void LookStick(const FInputActionValue& InputValue);

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

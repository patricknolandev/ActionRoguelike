// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutProjectile.h"
#include "GameFramework/Character.h"
#include "TutCharacter.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* AttachedVFX;
	
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepRadius;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepDistanceFallback;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ATutProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ATutProjectile> AbilityClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ATutProjectile> MobilityClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	
public:
	// Sets default values for this character's properties
	ATutCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UTutInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTutAttributeComponent* AttributeComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SpawnProjectile(TSubclassOf<ATutProjectile> ClassToSpawn);
	
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void AbilityBlackhole();
	void AbilityBlackhole_TimeElapsed();

	void MobilityDash();
	void MobilityDash_TimeElapsed();
	
	virtual void Jump() override;
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

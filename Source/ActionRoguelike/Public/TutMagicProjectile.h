// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TutProjectile.h"
#include "TutMagicProjectile.generated.h"

class UTutActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ATutMagicProjectile : public ATutProjectile
{
	GENERATED_BODY()
	
public:

	ATutMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bApplyEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UTutActionEffect> EffectActionClass;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
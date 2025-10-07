// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TutAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UTutAttributeComponent*, OwningComp, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTutAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UTutAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	UTutAttributeComponent();

protected:

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float HealthMax;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float RageMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float RageGrantedDmgPercent;

	// Use repnotify for state changes, ex. isAlive, item chests being open
	// Use multicast for a transient change, such as the health change event (we still replicate the health value)
	// IMPORTANT: Reliable netmulticasts do not consider relevancy
	// Relevancy: Unreal's handling of replication based on the distance to the player
	
	UFUNCTION(NetMulticast, Reliable) // TODO fix to be unreliable as input is currently disabled when health is 0 on character
	void MulticastHealthChanged(AActor* InstigatorActor, float NewValue, float Delta);

	UFUNCTION(NetMulticast, Reliable) // TODO fix to be unreliable as input is currently disabled when health is 0 on character
	void MulticastRageChanged(AActor* InstigatorActor, float NewValue, float Delta);

public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool RemoveRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsAtFullHealth() const;

	UFUNCTION(BlueprintCallable)
	bool IsLowHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool IsAtFullRage() const;

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	UFUNCTION(BlueprintCallable)
	float GetRageMax() const;
};

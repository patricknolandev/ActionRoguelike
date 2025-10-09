// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TutAction.generated.h"


class UTutActionComponent;
class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UTutAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	UTutActionComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UTutActionComponent* GetOwningComponent() const;

	/* Tags added to owning actor when activated, removed when action stops. */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();
	
public:

	void Initialize(UTutActionComponent* NewActionComp);
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override; //to give access in blueprint

	// support replication for uobjects
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};

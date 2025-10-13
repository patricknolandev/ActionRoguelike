// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TutPlayerState.generated.h"

class UTutSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, InstigatorActor, int32, NewCredits, int32,
                                               Delta);

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

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attributes")
	int32 Credits;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Attributes")
	int32 CreditsMax;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastCreditsChanged(AActor* InstigatorActor, int32 NewCredits, int32 Delta);

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static ATutPlayerState* GetPlayerState(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	bool IsAtFullCredits() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged; // use this in BP to update HUD

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UTutSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UTutSaveGame* SaveObject);
};

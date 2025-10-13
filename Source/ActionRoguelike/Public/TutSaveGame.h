// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TutSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTutSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Credits;
};

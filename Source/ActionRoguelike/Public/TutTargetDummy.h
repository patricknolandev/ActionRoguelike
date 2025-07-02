// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutTargetDummy.generated.h"

class UTutAttributeComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UTutAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth, float Delta);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutItemPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutItemPickup();

protected:

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

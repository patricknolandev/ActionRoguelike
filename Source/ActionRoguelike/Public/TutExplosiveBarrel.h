// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ATutExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutExplosiveBarrel();

protected:

	// Give barrel appearance
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	// Explosion force to be applied
	UPROPERTY(EditAnywhere)
	URadialForceComponent* ForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Apply explosive force
	UFUNCTION(BlueprintCallable)
	void ApplyImpulse(UPrimitiveComponent* HitComponent, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

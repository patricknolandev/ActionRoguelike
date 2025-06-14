// Fill out your copyright notice in the Description page of Project Settings.


#include "TutExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ATutExplosiveBarrel::ATutExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	MeshComp->OnComponentHit.AddDynamic(this, &ATutExplosiveBarrel::ApplyImpulse);
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ATutExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutExplosiveBarrel::ApplyImpulse(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
}

// Called every frame
void ATutExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


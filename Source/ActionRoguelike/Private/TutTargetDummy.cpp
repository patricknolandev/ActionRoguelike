// Fill out your copyright notice in the Description page of Project Settings.


#include "TutTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "TutAttributeComponent.h"

// Sets default values
ATutTargetDummy::ATutTargetDummy()
{
 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UTutAttributeComponent>("AttributeComp");
	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATutTargetDummy::OnHealthChanged);
	
}

void ATutTargetDummy::OnHealthChanged(AActor* InstigatorActor, UTutAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	// Target dummy is taking damage
	if (Delta < 0.0f)
	{
		// Hit flash on mesh
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	}
}

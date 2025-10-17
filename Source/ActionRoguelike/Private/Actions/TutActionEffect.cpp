// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/TutActionEffect.h"
#include "Actions/TutActionComponent.h"


UTutActionEffect::UTutActionEffect()
{
	bAutoStart = true;
}

void UTutActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UTutActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// If period is about to trigger in same tick, but the duration has ended, trigger it anyway
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UTutActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

float UTutActionEffect::GetTimeRemaining() const
{
	float EndTime = TimeStarted + Duration;
	return EndTime - GetWorld()->GetTimeSeconds();
}

void UTutActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

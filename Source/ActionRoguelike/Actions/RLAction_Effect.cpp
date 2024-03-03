// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAction_Effect.h"

#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "GameFramework/GameStateBase.h"

URLAction_Effect::URLAction_Effect()
	: Duration(0),
	  Period(0)
{
	bAutoStart = true;
}

void URLAction_Effect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		SetWorldTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		SetWorldTimer(PeriodHandle, Delegate, Period, true);	
	}
}

void URLAction_Effect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	ClearWorldTimer(PeriodHandle);
	ClearWorldTimer(DurationHandle);
	
	if (ActionComp != nullptr)
	{
		ActionComp->RemoveAction(this);
	}
}

float URLAction_Effect::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState != nullptr)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}
	
	return Duration;
}

void URLAction_Effect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}

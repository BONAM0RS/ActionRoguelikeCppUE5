// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAIController.h"


void ARLAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is NONE! Please assign BT in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

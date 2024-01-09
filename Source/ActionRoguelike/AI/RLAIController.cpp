// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAIController.h"

//#include "BehaviorTree/BlackboardComponent.h"
//#include "Kismet/GameplayStatics.h"


void ARLAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is NONE! Please assign BT in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// This feature now controls pawn sensing component
	// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (PlayerPawn != nullptr)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}

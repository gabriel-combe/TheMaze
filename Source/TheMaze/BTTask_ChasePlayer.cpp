// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the target location from the Blackboard via the Monster Controller
	const TObjectPtr<AMonsterAIController> mctrl = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!mctrl) return EBTNodeResult::Failed;

	const FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	// Move Monster to Player location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(mctrl, PlayerLocation);

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
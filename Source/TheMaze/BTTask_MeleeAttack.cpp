// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// If Out of Range do not attack
	const bool OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());

	// Get the MonsterAIController
	TObjectPtr<AMonsterAIController> mctrl = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (OutOfRange) {
		mctrl->IsAttacking = false;

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	mctrl->IsAttacking = true;

	// Finish task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
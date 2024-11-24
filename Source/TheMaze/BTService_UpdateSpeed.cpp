// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateSpeed.h"

UBTService_UpdateSpeed::UBTService_UpdateSpeed()
{
	NodeName = "Update Speed";

	bNotifyBecomeRelevant = true;
}

void UBTService_UpdateSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Get the target location from the Blackboard via the Monster Controller
	const TObjectPtr<AMonsterAIController> mctrl = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!mctrl) return;

	mctrl->MonsterAI->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}
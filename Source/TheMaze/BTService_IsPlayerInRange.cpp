// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInRange.h"
#include "MonsterAIController.h"
#include "MonsterAI.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerInRange::UBTService_IsPlayerInRange()
{
	NodeName = "Is Player In Range";

	bNotifyBecomeRelevant = true;
}

void UBTService_IsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get MonsterAIController and MonsterAI
	const TObjectPtr<AMonsterAIController> mctrl = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	const TObjectPtr<AMonsterAI> monster = Cast<AMonsterAI>(mctrl->GetPawn());

	// Get Player Character
	const TObjectPtr<ACharacter> player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Set the blackboard key value depending on the distance with the player
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		monster->GetDistanceTo(player) <= MeleeRange
	);
}
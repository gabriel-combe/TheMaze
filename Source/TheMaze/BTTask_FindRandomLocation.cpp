// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Monster and its AI controller
	const TObjectPtr<AMonsterAIController> mctrl = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (mctrl) {
		
		const TObjectPtr<APawn> monster = mctrl->GetPawn();
		if (monster) {

			// Get Monster location for the origin
			const FVector Origin = monster->GetActorLocation();

			// Get the Navmesh system and generate a random location
			const TObjectPtr<UNavigationSystemV1> NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (NavSys) {

				FNavLocation Loc;

				if (NavSys->GetRandomReachablePointInRadius(Origin, SearchRadius, Loc)) {

					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				// Finish task with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
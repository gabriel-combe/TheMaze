// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAIController.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateSpeed.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API UBTService_UpdateSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateSpeed();

	// Max Walk Speed of the AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxWalkSpeed;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

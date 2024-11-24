// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API UBTService_IsPlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	// Max Walk Speed of the AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MeleeRange = 50.0f;

public:
	UBTService_IsPlayerInRange();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

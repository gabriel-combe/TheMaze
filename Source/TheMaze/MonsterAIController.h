// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAI.h"
#include "TheMazeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);

	// Timer Handle for the stun
	FTimerHandle TimerHandleStun;

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	AMonsterAIController();

	// Tell if the player in in sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool IsPlayerInSight = false;

	// Tell if the monster is attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool IsAttacking = false;

	// Tell if the player in in sight
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TObjectPtr<AMonsterAI> MonsterAI;
};

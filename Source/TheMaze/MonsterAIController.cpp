// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
	SetupPerceptionSystem();
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const TObjectPtr<AMonsterAI> monster = Cast<AMonsterAI>(InPawn);
	if (!monster) return;
	
	const TObjectPtr<UBehaviorTree> tree = monster->GetBehaviourTree();
	if (!tree) return;

	UBlackboardComponent* b;
	UseBlackboard(tree->BlackboardAsset, b);
	Blackboard = b;
	RunBehaviorTree(tree);
}

void AMonsterAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (!SightConfig) return;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;
	SightConfig->SetMaxAge(15.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 830.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AMonsterAIController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	// Get Player character
	const TObjectPtr<ATheMazeCharacter> Player = Cast<ATheMazeCharacter>(Actor);
	if (!Player) return;

	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
}
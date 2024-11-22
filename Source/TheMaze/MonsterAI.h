// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "MonsterAI.generated.h"


UCLASS()
class THEMAZE_API AMonsterAI : public ACharacter
{
	GENERATED_BODY()

private:
	// Timer Handle for the stun
	FTimerHandle TimerHandleStun;

	// Blackboard of the controller
	TObjectPtr<UBlackboardComponent> Blackboard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> Tree;

public:	
	// Sets default values for this character's properties
	AMonsterAI();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get the Blackboard associated with the controller
	void SetBlackboard(UBlackboardComponent* b);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TObjectPtr<UBehaviorTree> GetBehaviourTree() const { return Tree; }

	// Called when we want to stun the AI
	UFUNCTION(BlueprintCallable, Category = "AI")
	void Stun(AActor* OverlappedComponent, AActor* OtherActor);

	// Called to reset the stun effect
	void ResetStun();
};

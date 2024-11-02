// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "TriggerSpikes.generated.h"

/**
 * 
 */
UCLASS()
class THEMAZE_API ATriggerSpikes : public ATriggerBox
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike Properties", meta = (AllowPrivateAccess = "true"))
	float damage = 10.0f;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SpikesMesh;

	UBoxComponent* BoxCollisionComponent;

protected:
	virtual void BeginPlay() override;

public:
	ATriggerSpikes();

	// Overlap Begin Event
	UFUNCTION(BlueprintCallable)
	void DamagePlayer(AActor* OverlappedComponent, AActor* OtherActor);

	// Overlap End Event
	UFUNCTION(BlueprintCallable)
	void ResetTimer(AActor* OverlappedComponent, AActor* OtherActor);

	// Apply the damage on the character
	void ApplyDamage(class ATheMazeCharacter* player);

};

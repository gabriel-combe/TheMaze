// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "HealthPackItem.generated.h"


UCLASS()
class THEMAZE_API AHealthPackItem : public AActor, public IInteractable
{
	GENERATED_BODY()

	// Mesh of the health pack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HealthPack;
	
public:	
	// Sets default values for this actor's properties
	AHealthPackItem();

	// Implement the Interact function of the interface
	void Interact_Implementation(ATheMazeCharacter* player) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

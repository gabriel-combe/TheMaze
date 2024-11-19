// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "MazeDataEnum.h"
#include "DoorInfoWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "DoorObject.generated.h"

UCLASS()
class THEMAZE_API ADoorObject : public AActor, public IInteractable
{
	GENERATED_BODY()

private:
	// Scene Component
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// Mesh component of the left door
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorLComp;

	// Mesh component of the right door
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorRComp;

	// Mesh component of the top of the door
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorTopComp;

	// Widget component of the door (for text display)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetDoor;

	// User Widget Class to use for the door info
	UPROPERTY(EditAnywhere, Category = "Tier", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDoorInfoWidget> DoorInfoClass;

	// User Widget to use for the door info
	TObjectPtr<UDoorInfoWidget> DoorInfo;

	// Tell if the door is open
	bool Open;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Tier of the door
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tier")
	EKeyDoorTier DoorTier;

	// Required number of key
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tier")
	int RequiredKey;

	// Sets default values for this actor's properties
	ADoorObject();

	// Implement the Interact function of the interface
	void Interact_Implementation(ATheMazeCharacter* player) override;

	// Set Door tier
	void SetTier(EKeyDoorTier doorTier);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update the text of the door
	void UpdateDoorText();

};

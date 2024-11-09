// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "MazeData.h"
#include "DoorInfoWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "DoorObject.generated.h"

UCLASS()
class THEMAZE_API ADoorObject : public AActor, public IInteractable
{
	GENERATED_BODY()

	// Scene Component
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// Mesh component of the left door
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorLComp;

	// Mesh component of the right door
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorRComp;

	// Widget component of the door (for text display)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tier", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetDoor;

	//// User Widget to use for the door info
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UDoorInfoWidget> DoorInfoClass;

	//// User Widget to use for the door info
	TObjectPtr<UDoorInfoWidget> DoorInfo;

	// Tell if the door is open
	bool Open;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update the text of the door
	void UpdateDoorText();

};

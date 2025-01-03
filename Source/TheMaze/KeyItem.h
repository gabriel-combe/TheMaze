// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "MazeDataEnum.h"
#include "GameFramework/Actor.h"
#include "KeyItem.generated.h"

UCLASS()
class THEMAZE_API AKeyItem : public AActor, public IInteractable
{
	GENERATED_BODY()

private:
	// Scene Component
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// Mesh component of the key
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemComp;

	// Mesh of the common key
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* KeyItemCommon = nullptr;

	// Mesh of the uncommon key
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* KeyItemUncommon = nullptr;

	// Mesh of the rare key
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* KeyItemRare = nullptr;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	

	// Tier of the key
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tier")
	EKeyDoorTier KeyTier;

	// Sets default values for this actor's properties
	AKeyItem();

	// Implement the Interact function of the interface
	void Interact_Implementation(ATheMazeCharacter* player) override;

	// Set Key tier
	void SetTier(EKeyDoorTier keyTier);

	// Update the key mesh based on the key tier
	UFUNCTION(BlueprintCallable, Category = "Tier")
	void UpdateKeyMesh();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CeilingLight.generated.h"

UCLASS()
class THEMAZE_API ACeilingLight : public AActor
{
	GENERATED_BODY()
	
private:	
	// Scene Component
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// Material instance dynamic of the light to change its emissive
	TObjectPtr<UMaterialInstanceDynamic> MILight;

	// Mesh of the light
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FluorescentLight;

	// Flickering probability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	float FlickerProba = 0.5f;

	// Light Off probability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	float LightOffProba = 0.5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACeilingLight();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to change the emissive of the light
	UFUNCTION(BlueprintCallable)
	void ChangeLightEmissive(float Emissive);

	// Called to activate the light flickering
	UFUNCTION(BlueprintCallable)
	void ActivateLightFlickering(bool Activate, float speed);
};

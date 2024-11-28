// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingLight.h"

// Sets default values
ACeilingLight::ACeilingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	FluorescentLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fluorescent Light"));
	FluorescentLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACeilingLight::BeginPlay()
{
	Super::BeginPlay();

	if (!FluorescentLight) return;

	MILight = UMaterialInstanceDynamic::Create(FluorescentLight->GetMaterial(0), FluorescentLight);
	FluorescentLight->SetMaterial(0, MILight);
	
	if (FMath::SRand() < FlickerProba) {
		ActivateLightFlickering(true, (FMath::SRand() * 0.8) + 0.1);
	}
	else {
		if (FMath::SRand() < LightOffProba) {
			ChangeLightEmissive(0.1f);
		}
		else {
			ChangeLightEmissive(4.0f);
		}
	}
}

// Called every frame
void ACeilingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to change the emissive of the light
void ACeilingLight::ChangeLightEmissive(float Emissive)
{
	if (!MILight || !FluorescentLight) return;

	MILight->SetScalarParameterValue(FName("Emissive"), Emissive);
}

// Called to activate the light flickering
void ACeilingLight::ActivateLightFlickering(bool Activate, float speed)
{
	if (!MILight || !FluorescentLight) return;

	MILight->SetScalarParameterValue(FName("Flickering"), Activate);
	MILight->SetScalarParameterValue(FName("Speed"), speed);
}
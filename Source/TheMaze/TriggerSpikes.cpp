// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerSpikes.h"

ATriggerSpikes::ATriggerSpikes()
{

	/*SpikesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spikes"));
	SpikesMesh->SetupAttachment(RootComponent);
	SpikesMesh->SetMobility(EComponentMobility::Static);
	SpikesMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SpikesMesh->SetGenerateOverlapEvents(false);*/

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeDefault(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeDefault.Succeeded()) {
		SpikesMesh->SetStaticMesh(ConeDefault.Object);
		SpikesMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}*/


	// Register Events
	OnActorBeginOverlap.AddDynamic(this, &ATriggerSpikes::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ATriggerSpikes::OnEndOverlap);
}

void ATriggerSpikes::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "BEGIN OVERLAP");

	if (!OtherActor->ActorHasTag("Player")) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "BEGIN OVERLAP");
}

void ATriggerSpikes::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "END OVERLAP");
}
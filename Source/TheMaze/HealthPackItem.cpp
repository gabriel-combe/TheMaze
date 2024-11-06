// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPackItem.h"

// Sets default values
AHealthPackItem::AHealthPackItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthPack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthPack"));
	HealthPack->SetupAttachment(RootComponent);
	//HealthPack->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//HealthPack->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereDefault(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereDefault.Succeeded()) {
		HealthPack->SetStaticMesh(SphereDefault.Object);
		HealthPack->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		HealthPack->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

}

// Called when the game starts or when spawned
void AHealthPackItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPackItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Implement the Interact function of the interface
void AHealthPackItem::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "HEALTHPACK INCOMING");

	bool success = player->FullHealCharacter();

	this->SetActorHiddenInGame(success);

	if (success)
		Destroy(true);
}

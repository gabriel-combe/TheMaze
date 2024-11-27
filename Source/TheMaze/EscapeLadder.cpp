// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeLadder.h"

// Sets default values
AEscapeLadder::AEscapeLadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	Ladder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EscapeLadder"));
	Ladder->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeDefault(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeDefault.Succeeded()) {
		Ladder->SetStaticMesh(CubeDefault.Object);
		Ladder->SetRelativeScale3D(FVector(0.25f, 0.5f, 4.0f));
		Ladder->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AEscapeLadder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeLadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Implement the Interact function of the interface
void AEscapeLadder::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "ESCAPED");

	player->SetWin();
}


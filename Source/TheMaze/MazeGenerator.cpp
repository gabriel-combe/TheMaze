// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	// Create the floor mesh component
	ISMFloorComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM Floors"));
	ISMFloorComponent->SetupAttachment(RootComponent);

	// Create the wall mesh component
	ISMWallComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM Walls"));
	ISMWallComponent->SetupAttachment(ISMFloorComponent);

	Width = 25;
	Height = 25;

	// Setup the possible direction
	PossibleDirection.Emplace(FVector2D(-1, 0));
	PossibleDirection.Emplace(FVector2D(0, -1));
	PossibleDirection.Emplace(FVector2D(1, 0));
	PossibleDirection.Emplace(FVector2D(0, 1));
}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the maze game instance
	MazeGI = CastChecked<UMazeGameInstance>(GetGameInstance());

	// Set the default Width and Height of the maze from the game instance
	Width = MazeGI->Size[0];
	Height = MazeGI->Size[1];

	Origin = FVector2D(Width - 1, Height - 1);

	NewMazeMap();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d	%d"), PossibleDirection[0].X, PossibleDirection[0].Y));
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Generate a new Maze map (default map)
void AMazeGenerator::NewMazeMap()
{
	// Empty Array
	MazeMap.Empty();

	// Clear all the instances
	ISMFloorComponent->ClearInstances();
	ISMWallComponent->ClearInstances();

	for (int y = 0; y < Height; y++) {
		MazeMap.Emplace(FNode(FVector2D(0, y), FVector2D(1, 0), true));
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(0, y * (CellSize + 25.0f), 0.0f), FVector::OneVector));

		for (int x = 1; x < Width - 1; x++) {
			MazeMap.Emplace(FNode(FVector2D(x, y), FVector2D(1, 0)));
			MazeMap[x + y * Width].SetLinkNbOthers(1);

			ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(x * (CellSize + 25.0f), y * (CellSize + 25.0f), 0.0f), FVector::OneVector));
		}

		MazeMap.Emplace(FNode(FVector2D(Width-1, y), FVector2D(0, 1)));
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector((Width - 1) * (CellSize + 25.0f), y * (CellSize + 25.0f), 0.0f), FVector::OneVector));

		if (y != 0)
			MazeMap[Width-1 + y * Width].SetLinkNbOthers(2);
	}

	MazeMap[(Width - 1) + (Height - 1) * Width].SetDirection(FVector2D(0, 0));

	//ISMFloorComponent->InstanceBodies
}
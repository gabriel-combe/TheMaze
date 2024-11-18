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

	Width = 5;
	Height = 5;

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

// TODO Might need to refactor this function (my eyes are bleeding)
// Generate a new Maze map (default map)
void AMazeGenerator::NewMazeMap()
{
	// Empty Array
	MazeMap.Empty();

	// Clear all the instances
	ISMFloorComponent->ClearInstances();
	ISMWallComponent->ClearInstances();

	// Generate the outer walls of the maze
	/*for (int y = 0; y < Height; y++)
		ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f + (Width-1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = Width-1; x >= 0; x--)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, 100.0f + (Height-1) * CellSize, 0.0f), FVector::OneVector));

	for (int y = Height-1; y >= 0; y--)
		ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(-100.0f, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = 0; x < Width; x++)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, -100.0f, 0.0f), FVector::OneVector));*/

	int CurrentWallIndex = -1;
	int WallIndexOffset = 2 * (Width + Height);

	// Generate the floor and some walls of the maze
	for (int y = 0; y < Height; y++) {
		FNode CurrentNode = FNode(FVector2D(0, y), FVector2D(1, 0), true);
		
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(0, y * CellSize, 0.0f), FVector::OneVector));
		
		// North Wall at X start
		if (y != (Height - 1)) {
			CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(0, 100.0f + y * CellSize, 0.0f), FVector::OneVector));
			CurrentNode.WallArray[int(EDirection::NORTH)] = CurrentWallIndex;
		}

		// South Wall at X start
		if (y != 0) {
			CurrentNode.WallArray[int(EDirection::SOUTH)] = 2 * ((y - 1) * Width);
			if (y == (Height - 1))
				CurrentNode.WallArray[int(EDirection::SOUTH)]--;
		}
			
		// West Wall at X start
		CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f, y * CellSize, 0.0f), FVector::OneVector));
		CurrentNode.WallArray[int(EDirection::WEST)] = CurrentWallIndex;

		// Add the node in the maze map
		MazeMap.Emplace(CurrentNode);

		// Set initial wall display for the path (Hide or not)
		ISMWallComponent->UpdateInstanceTransform(MazeMap[(Width - 1) + y * Width].WallArray[int(EDirection::NORTH)], FTransform(FQuat::Identity, FVector(0, 0, -300), FVector::OneVector));
		//ISMWallComponent->InstanceBodies[MazeMap[y * Width].WallArray[int(EDirection::WEST)]]->UpdateBodyScale(FVector(0, 0, 0));

		for (int x = 1; x < Width - 1; x++) {
			CurrentNode = FNode(FVector2D(x, y), FVector2D(1, 0));

			ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(x * CellSize, y * CellSize, 0.0f), FVector::OneVector));
			
			// East Wall
			CurrentNode.WallArray[int(EDirection::EAST)] = CurrentWallIndex;

			// North Wall
			if (y != (Height - 1)) {
				CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, 100.0f + y * CellSize, 0.0f), FVector::OneVector));
				CurrentNode.WallArray[int(EDirection::NORTH)] = CurrentWallIndex;
			}

			// South Wall
			if (y != 0) {
				CurrentNode.WallArray[int(EDirection::SOUTH)] = 2 * (x + (y - 1) * Width);
				if (y == (Height - 1))
					CurrentNode.WallArray[int(EDirection::SOUTH)]--;
			}
			
			// West Wall
			CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f + x * CellSize, y * CellSize, 0.0f), FVector::OneVector));
			CurrentNode.WallArray[int(EDirection::WEST)] = CurrentWallIndex;

			MazeMap.Emplace(CurrentNode);
			MazeMap[x + y * Width].SetLinkNbOthers(1);

			// Set initial wall display for the path (Hide or not)
			ISMWallComponent->UpdateInstanceTransform(MazeMap[(Width - 1) + y * Width].WallArray[int(EDirection::NORTH)], FTransform(FQuat::Identity, FVector(0, 0, -300), FVector::OneVector));
			//ISMWallComponent->InstanceBodies[MazeMap[x + y * Width].WallArray[int(EDirection::WEST)]]->UpdateBodyScale(FVector(0, 0, 0));
		}

		CurrentNode = FNode(FVector2D(Width - 1, y), FVector2D(0, 1));
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector((Width - 1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

		// East Wall at X end
		CurrentNode.WallArray[int(EDirection::EAST)] = CurrentWallIndex;

		// North Wall at X end
		if (y != (Height - 1)) {
			CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector((Width - 1) * CellSize, 100.0f + y * CellSize, 0.0f), FVector::OneVector));
			CurrentNode.WallArray[int(EDirection::NORTH)] = CurrentWallIndex;
		}

		// South Wall
		if (y != 0) {
			CurrentNode.WallArray[int(EDirection::SOUTH)] = 2 * ((Width - 1) + (y - 1) * Width);
			if (y == (Height - 1))
				CurrentNode.WallArray[int(EDirection::SOUTH)]--;
		}

		MazeMap.Emplace(CurrentNode);
		if (y != 0) {
			MazeMap[Width-1 + y * Width].SetLinkNbOthers(2);

			// Set initial wall display for the path (Hide or not)
			if (y != (Height - 1))
				ISMWallComponent->UpdateInstanceTransform(MazeMap[(Width - 1) + y * Width].WallArray[int(EDirection::NORTH)], FTransform(FQuat::Identity, FVector(0, 0, -300), FVector::OneVector));
				//ISMWallComponent->InstanceBodies[MazeMap[(Width - 1) + y * Width].WallArray[int(EDirection::NORTH)]]->UpdateBodyScale(FVector(0, 0, 0));
		}
	}

	MazeMap[(Width - 1) + (Height - 1) * Width].SetDirection(FVector2D(0, 0));

	
}

// One step iteration of the maze generation
void AMazeGenerator::MazeGenIteration()
{

}
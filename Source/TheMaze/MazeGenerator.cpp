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

	// Sphere Test setup
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	SphereMesh->SetStaticMesh(SphereMeshAsset.Object);

	Width = 10;
	Height = 10;
	NumberOfMonster = 2;
	ProbaTriggerSpikes = 0.2;

	MaxDeadEnd = int(Width * Height * 0.2f);

	ListNumberKeyByTier.Init(0, StaticEnum<EKeyDoorTier>()->GetMaxEnumValue());
	ListNumberKeyByTier[2] = 3;

	// Setup the possible direction
	PossibleDirection.Emplace(FVector2D(0, 1));
	PossibleDirection.Emplace(FVector2D(-1, 0));
	PossibleDirection.Emplace(FVector2D(0, -1));
	PossibleDirection.Emplace(FVector2D(1, 0));
}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the maze game instance
	MazeGI = CastChecked<UMazeGameInstance>(GetGameInstance());

	// Set the default parameters of the maze from the game instance
	Width = MazeGI->Size[0];
	Height = MazeGI->Size[1];
	NumberOfMonster = MazeGI->NBEnemies;

	NewMazeMap();

	MazeGenMultiStepRandomize();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d	%d"), PossibleDirection[0].X, PossibleDirection[0].Y));
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGenerator::GenerateOuterWalls()
{
	// Generate the outer walls of the maze
	for (int y = 0; y < Height; y++)
		ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f + (Width - 1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = Width - 1; x >= 0; x--)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, 100.0f + (Height - 1) * CellSize, 0.0f), FVector::OneVector));

	for (int y = Height - 1; y >= 0; y--)
		ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(-100.0f, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = 0; x < Width; x++)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, -100.0f, 0.0f), FVector::OneVector));
}

void AMazeGenerator::GenerateNodeWalls(FNode& CurrentNode, int CurrentWallIndex, int WallIndexOffset)
{
	// East Wall
	if (CurrentNode.Position.X != 0) {
		FNode EastNode = MazeMap[(CurrentNode.Position.X - 1) + CurrentNode.Position.Y * Width];
		CurrentNode.WallArray[int(EDirection::EAST)] = EastNode.WallArray[int(EDirection::WEST)];
	}

	// North Wall
	if (CurrentNode.Position.Y != (Height - 1)) {
		CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(CurrentNode.Position.X * CellSize, 100.0f + CurrentNode.Position.Y * CellSize, 0.0f), FVector::OneVector));
		CurrentNode.WallArray[int(EDirection::NORTH)] = CurrentWallIndex;
	}

	// South Wall
	if (CurrentNode.Position.Y != 0) {
		FNode SouthNode = MazeMap[CurrentNode.Position.X + (CurrentNode.Position.Y - 1) * Width];
		CurrentNode.WallArray[int(EDirection::SOUTH)] = SouthNode.WallArray[int(EDirection::NORTH)];
	}

	// West Wall
	if (CurrentNode.Position.X != Width - 1) {
		CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f + CurrentNode.Position.X * CellSize, CurrentNode.Position.Y * CellSize, 0.0f), FVector::OneVector));
		CurrentNode.WallArray[int(EDirection::WEST)] = CurrentWallIndex;
	}
}

void AMazeGenerator::MoveWallByDir(FNode& Node, EDirection dir, float ZMove)
{
	FVector2D factor = FVector2D(0, 0);

	switch (dir)
	{
	case EDirection::NORTH:
		factor.Y = 1;
		break;
	case EDirection::EAST:
		factor.X = -1;
		break;
	case EDirection::SOUTH:
		factor.Y = -1;
		break;
	case EDirection::WEST:
		factor.X = 1;
		break;
	case EDirection::UNDEF:
		return;
	default:
		return;
	}

	ISMWallComponent->UpdateInstanceTransform(Node.WallArray[int(dir)], FTransform(FRotator(0, FMath::Abs(factor.Y) * 90, 0), FVector(factor.X * 100.0f + Node.Position.X * CellSize, factor.Y * 100.0f + Node.Position.Y * CellSize, ZMove), FVector::OneVector));
}

void AMazeGenerator::HideWallByDir(FNode& Node, EDirection dir)
{
	MoveWallByDir(Node, dir, -300);
}

void AMazeGenerator::ShowWallByDir(FNode& Node, EDirection dir)
{
	MoveWallByDir(Node, dir, 0);
}

bool AMazeGenerator::IsPosInBound(FVector2D pos)
{
	if (pos.X < 0 || pos.X >= Width || pos.Y < 0 || pos.Y >= Height)
		return false;

	return true;
}

EDirection AMazeGenerator::VecToEDir(FVector2D direction)
{
	int Index = PossibleDirection.Find(direction);

	switch (Index)
	{
	case 0:
		return EDirection::NORTH;
	case 1:
		return EDirection::EAST;
	case 2:
		return EDirection::SOUTH;
	case 3:
		return EDirection::WEST;
	default:
		return EDirection::UNDEF;
	}
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
	GenerateOuterWalls();

	int CurrentWallIndex = -1;
	int WallIndexOffset = 2 * (Width + Height) - 1;

	// Generate the floor and some walls of the maze
	for (int y = 0; y < Height; y++) {
		FNode CurrentNode = FNode(FVector2D(0, y), FVector2D(1, 0), true);
		ListUnpopulatedDeadEnd.Emplace(CurrentNode);
		
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(0, y * CellSize, 0.0f), FVector::OneVector));

		// Generate the wall of a node
		GenerateNodeWalls(CurrentNode, CurrentWallIndex, WallIndexOffset);

		// Add the node in the maze map
		MazeMap.Emplace(CurrentNode);

		// Set initial wall display for the path (Hide or not)
		ISMWallComponent->UpdateInstanceTransform(CurrentNode.WallArray[int(EDirection::WEST)], FTransform(FQuat::Identity, FVector(100.0f + CurrentNode.Position.X * CellSize, CurrentNode.Position.Y * CellSize, -300), FVector::OneVector));

		for (int x = 1; x < Width - 1; x++) {
			CurrentNode = FNode(FVector2D(x, y), FVector2D(1, 0));

			ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(x * CellSize, y * CellSize, 0.0f), FVector::OneVector));

			// Generate the wall of a node
			GenerateNodeWalls(CurrentNode, CurrentWallIndex, WallIndexOffset);

			MazeMap.Emplace(CurrentNode);
			MazeMap[x + y * Width].SetLinkNbOthers(1);

			// Set initial wall display for the path (Hide or not)
			HideWallByDir(CurrentNode, EDirection::WEST);
		}

		CurrentNode = FNode(FVector2D(Width - 1, y), FVector2D(0, 1));
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector((Width - 1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

		// Generate the wall of a node
		GenerateNodeWalls(CurrentNode, CurrentWallIndex, WallIndexOffset);

		MazeMap.Emplace(CurrentNode);
		if (y != 0)
			MazeMap[Width-1 + y * Width].SetLinkNbOthers(2);

		// Set initial wall display for the path (Hide or not)
		if (y != (Height - 1))
			HideWallByDir(CurrentNode, EDirection::NORTH);
	}

	MazeMap[(Width - 1) + (Height - 1) * Width].SetDirection(FVector2D(0, 0));
	Origin = FVector2D(Width - 1, Height - 1);

	SphereMesh->SetRelativeLocation(FVector((Width - 1) * CellSize, (Height - 1) * CellSize, 0));	
}

// One step iteration of the maze generation
void AMazeGenerator::MazeGenIteration()
{
	// Select a random direction
	int Index = FMath::RandRange(0, PossibleDirection.Num() - 1);
	FVector2D direction = PossibleDirection[Index];

	NextOrigin = Origin + direction;
	if (!IsPosInBound(NextOrigin)) return;

	// Recompute if the previous pointing node is a dead end
	FVector2D PrevDir = MazeMap[NextOrigin.X + NextOrigin.Y * Width].LinkDirection;
	FVector2D PrevNodePos = NextOrigin + PrevDir;
	FNode* TargetNextNode = &MazeMap[PrevNodePos.X + PrevNodePos.Y * Width];
	MazeMap[PrevNodePos.X + PrevNodePos.Y * Width].RemoveLinkNbOthers();

	// NextOrigin node and Origin Node
	FNode* NextNode = &MazeMap[NextOrigin.X + NextOrigin.Y * Width];
	FNode* OriginNode = &MazeMap[Origin.X + Origin.Y * Width];
	bool OriginWasDeadEnd = OriginNode->isDeadEnd;
	bool NextOriginWasDeadEnd = NextNode->isDeadEnd;

	// Show wall
	ShowWallByDir(*NextNode, VecToEDir(PrevDir));

	// Set the origin node direction to this direction
	OriginNode->SetDirection(direction);

	// Hide wall
	HideWallByDir(*OriginNode, VecToEDir(direction));
	

	// Reset the direction of the new origin
	Origin = NextOrigin;
	NextNode->SetDirection(FVector2D(0, 0));
	NextNode->AddLinkNbOthers();

	if (OriginNode->isDeadEnd)
		ListUnpopulatedDeadEnd.AddUnique(*OriginNode);

	if (NextNode->isDeadEnd)
		ListUnpopulatedDeadEnd.AddUnique(*NextNode);

	if (TargetNextNode->isDeadEnd)
		ListUnpopulatedDeadEnd.AddUnique(*TargetNextNode);

	// Rearrange Items and Doors in the dead ends
	if (!NextNode->Item.IsNull() && NextOriginWasDeadEnd && NextNode->isDeadEnd) {
		if (TargetNextNode->isDeadEnd && TargetNextNode->Item.IsNull()) {
			TargetNextNode->Door = NextNode->Door;
			TargetNextNode->Item = NextNode->Item;

			ListPopulatedDeadEnd.Emplace(*TargetNextNode);

			TargetNextNode->UpdateTransformItem();
		}
		else {
			FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
			ListUnpopulatedDeadEnd.Remove(RandNode);

			FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

			PtrRandNode->Door = NextNode->Door;
			PtrRandNode->Item = NextNode->Item;

			ListPopulatedDeadEnd.Emplace(*PtrRandNode);

			PtrRandNode->UpdateTransformItem();
		}

		ListPopulatedDeadEnd.Remove(*NextNode);
		ListUnpopulatedDeadEnd.Emplace(*NextNode);
	}

	if (!OriginNode->Item.IsNull() && OriginWasDeadEnd && !OriginNode->isDeadEnd) {
		if (NextNode->isDeadEnd && NextNode->Item.IsNull()) {
			NextNode->Door = OriginNode->Door;
			NextNode->Item = OriginNode->Item;

			ListPopulatedDeadEnd.Emplace(*NextNode);

			NextNode->UpdateTransformItem();
		}
		else if (TargetNextNode->isDeadEnd && TargetNextNode->Item.IsNull()) {
			TargetNextNode->Door = OriginNode->Door;
			TargetNextNode->Item = OriginNode->Item;

			ListPopulatedDeadEnd.Emplace(*TargetNextNode);

			TargetNextNode->UpdateTransformItem();
		}
		else {
			FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
			ListUnpopulatedDeadEnd.Remove(RandNode);

			FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

			RandNode.Door = OriginNode->Door;
			RandNode.Item = OriginNode->Item;

			ListPopulatedDeadEnd.Emplace(*PtrRandNode);

			RandNode.UpdateTransformItem();
		}

		ListPopulatedDeadEnd.Remove(*OriginNode);
	}

	// Test Sphere Move
	SphereMesh->SetRelativeLocation(FVector(Origin.X * CellSize, Origin.Y * CellSize, 0));
}

void AMazeGenerator::MazeGenMultiStepRandomize()
{
	for (int i = 0; i < Width * Height * 10; i++)
		MazeGenIteration();
}

void AMazeGenerator::MonsterAISpawn()
{
	MonsterAIClear();

	for (int i = 0; i < NumberOfMonster; i++) {
		bool ValidPos = false;
		FNode Node;

		while (!ValidPos) {
			Node = MazeMap[FMath::RandRange(0, MazeMap.Num() - 1)];

			ValidPos = !Node.isDeadEnd;
		}

		ListMonsters.Emplace(GetWorld()->SpawnActor<AMonsterAI>(MonsterAIBP, FTransform(GetActorRotation(), GetActorLocation() + FVector(Node.Position.X * CellSize, Node.Position.Y * CellSize, 90), FVector::OneVector)));
	}
}

void AMazeGenerator::MonsterAIClear()
{
	if (ListMonsters.IsEmpty()) return;

	for (AMonsterAI* monster : ListMonsters) {
		monster->Destroy();
	}

	ListMonsters.Empty();
}

void AMazeGenerator::TriggerSpikesSpawn()
{
	TriggerSpikesClear();

	for (int i = 0; i < MazeMap.Num(); i++)
	{
		FNode Node = MazeMap[i];

		if (Node.isDeadEnd || FMath::SRand() > ProbaTriggerSpikes) continue;

		ListTriggerSpikes.Emplace(GetWorld()->SpawnActor<ATriggerSpikes>(TriggerSpikesBP, FTransform(GetActorRotation(), GetActorLocation() + FVector(Node.Position.X * CellSize, Node.Position.Y * CellSize, 40), FVector::OneVector)));
	}
}

void AMazeGenerator::TriggerSpikesClear()
{
	if (ListTriggerSpikes.IsEmpty()) return;

	for (ATriggerSpikes* spikes : ListTriggerSpikes) {
		spikes->Destroy();
	}

	ListTriggerSpikes.Empty();
}

// test gen key 
void AMazeGenerator::SpawnKeyDoor()
{
	for (int i = 0; i < ListNumberKeyByTier[int(EKeyDoorTier::KeyDoor_Rare)]; i++) {
		KeySpawn(EKeyDoorTier::KeyDoor_Rare);
	}
}

// Spawn the Monster AI in the maze
void AMazeGenerator::KeySpawn(EKeyDoorTier Tier)
{
	FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
	ListUnpopulatedDeadEnd.Remove(RandNode);

	FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

	AKeyItem* key = GetWorld()->SpawnActor<AKeyItem>(KeyBP);
	key->KeyTier = Tier;
	RandNode.Item = key;

	switch (Tier)
	{
	case EKeyDoorTier::KeyDoor_Uncommon:
		ADoorObject* door = GetWorld()->SpawnActor<ADoorObject>(DoorBP);
		door->DoorTier = EKeyDoorTier::KeyDoor_Common;
		RandNode.Door = door;
		break;
	case EKeyDoorTier::KeyDoor_Rare:
		ADoorObject* door = GetWorld()->SpawnActor<ADoorObject>(DoorBP);
		door->DoorTier = EKeyDoorTier::KeyDoor_Uncommon;
		RandNode.Door = door;
		break;
	default:
		break;
	}

	ListPopulatedDeadEnd.Emplace(*PtrRandNode);
}
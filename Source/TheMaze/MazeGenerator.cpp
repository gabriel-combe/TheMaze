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

	// Sphere and Cube Test setup
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	SphereMesh->SetStaticMesh(SphereMeshAsset.Object);

	CubeInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cube Test"));
	CubeInstance->SetupAttachment(RootComponent);

	Width = 15;
	Height = 15;
	NumberOfMonster = 2;
	EvolutionTime = 10;
	ProbaTriggerSpikes = 0.1;

	MaxDeadEnd = int(Width * Height * 0.2f);

	ListNumberKeyByTier.Init(0, StaticEnum<EKeyDoorTier>()->GetMaxEnumValue());
	ListNumberKeyByTier[2] = 3;
	ListNumberKeyByTier[1] = 2 * ListNumberKeyByTier[2];
	ListNumberKeyByTier[0] = 2 * ListNumberKeyByTier[1];

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
	EvolutionTime = MazeGI->EvolutionTime;
	NumberOfRareKey = MazeGI->NBRareKey;

	ListNumberKeyByTier.Init(0, StaticEnum<EKeyDoorTier>()->GetMaxEnumValue());
	ListNumberKeyByTier[2] = NumberOfRareKey;
	ListNumberKeyByTier[1] = 3 * ListNumberKeyByTier[2];
	ListNumberKeyByTier[0] = 3 * ListNumberKeyByTier[1];

	// Generate the base Maze
	NewMazeMap();

	// Fully randomize the Maze
	MazeGenMultiStepRandomize();

	// Spawn the Keys and Doors
	SpawnKeyDoor();

	// Spawn Health Pack

	// Spawn the Trigger Spikes
	TriggerSpikesSpawn();

	// Spawn the MonsterAI
	MonsterAISpawn(); 

	// Create a timer to make the Maze evolve
	FTimerHandle TimerHandleMazeEvolution;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMazeGenerator::MazeGenEvolution);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleMazeEvolution, Delegate, EvolutionTime, true);

	// Spawn Start and End
	SpawnStartEnd();

	// Spawn Player
	SpawnPlayer();
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Create the outer walls of the maze
void AMazeGenerator::GenerateOuterWalls()
{
	// Generate the outer walls of the maze
	for (int y = 0; y < Height; y++)
		ISMWallComponent->AddInstance(FTransform(FQuat::Identity, FVector(100.0f + (Width - 1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = Width - 1; x >= 0; x--)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(x * CellSize, 100.0f + (Height - 1) * CellSize, 0.0f), FVector::OneVector));

	for (int y = Height - 1; y >= 0; y--)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 180.0f, 0.0f), FVector(-100.0f, y * CellSize, 0.0f), FVector::OneVector));

	for (int x = 0; x < Width; x++)
		ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 270.0f, 0.0f), FVector(x * CellSize, -100.0f, 0.0f), FVector::OneVector));
}

// Generate the walls for a node
void AMazeGenerator::GenerateNodeWalls(FNode& CurrentNode, int CurrentWallIndex)
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
		CurrentWallIndex = ISMWallComponent->AddInstance(FTransform(FRotator(0.0f, 180.0f, 0.0f), FVector(100.0f + CurrentNode.Position.X * CellSize, CurrentNode.Position.Y * CellSize, 0.0f), FVector::OneVector));
		CurrentNode.WallArray[int(EDirection::WEST)] = CurrentWallIndex;
	}
}

// Move a wall on the Z axis
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

// Hide a wall
void AMazeGenerator::HideWallByDir(FNode& Node, EDirection dir)
{
	MoveWallByDir(Node, dir, -300);
}

// Show a wall
void AMazeGenerator::ShowWallByDir(FNode& Node, EDirection dir)
{
	MoveWallByDir(Node, dir, 0);
}

// Check if a position is in the Maze
bool AMazeGenerator::IsPosInBound(FVector2D pos)
{
	if (pos.X < 0 || pos.X >= Width || pos.Y < 0 || pos.Y >= Height)
		return false;

	return true;
}

// Direction to EDirection enum
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

// Find the direction of the open wall for dead end node
FVector2D AMazeGenerator::FindDirDeadEnd(FNode* DeadEnd)
{
	FVector2D DeadEndDir = DeadEnd->LinkDirection;

	if (DeadEndDir.IsZero()) {
		for (FVector2D NodeDir : PossibleDirection) {
			if (!IsPosInBound(DeadEnd->Position + NodeDir)) continue;

			FNode Neighbour = MazeMap[GetNeighbourViaDirection(DeadEnd->Position, NodeDir)];

			if (MazeMap[GetNeighbourViaDirection(Neighbour.Position, Neighbour.LinkDirection)] != *DeadEnd) continue;

			DeadEndDir = PossibleDirection[(PossibleDirection.Find(Neighbour.LinkDirection) + 2) % 4]; // Ugly
		}
	}

	return DeadEndDir;
}

// Update the node for dead end cases
void AMazeGenerator::UpdateNodeForDeadEnd(FNode* Node)
{
	// Remove node from unpopulated dead end list
	if (!Node->isDeadEnd) {
		ListUnpopulatedDeadEnd.Remove(*Node);
		return;
	}
	
	// Update Spike transform if the node is now a dead end
	if (Node->Spike) {
		bool ValidPos = false;
		FNode* NewNode = nullptr;

		while (!ValidPos) {
			NewNode = &MazeMap[FMath::RandRange(0, MazeMap.Num() - 1)];

			ValidPos = !NewNode->isDeadEnd;
		}

		Node->Spike->SetActorRelativeTransform(FTransform(GetActorRotation(), GetActorLocation() + FVector(NewNode->Position.X * CellSize, NewNode->Position.Y * CellSize, 40), FVector::OneVector));

		NewNode->Spike = Node->Spike;
		Node->Spike = nullptr;
	}

	// Remove the node from the list of unpopulated dead end if it has an item
	if (!Node->HasItem) {
		ListUnpopulatedDeadEnd.AddUnique(*Node);
		return;
	}

	Node->UpdateTransformItem(FindDirDeadEnd(Node), CellSize);
}

// Generate a new Maze map (default map)
void AMazeGenerator::NewMazeMap()
{
	// Empty Array
	MazeMap.Empty();
	ListUnpopulatedDeadEnd.Empty();
	ListPopulatedDeadEnd.Empty();

	// Clear all the instances
	ISMFloorComponent->ClearInstances();
	ISMWallComponent->ClearInstances();
	CubeInstance->ClearInstances();

	// Generate the outer walls of the maze
	GenerateOuterWalls();

	int CurrentWallIndex = -1;

	// Generate the floor and some walls of the maze
	for (int y = 0; y < Height; y++) {
		FNode CurrentNode = FNode(FVector2D(0, y), FVector2D(1, 0), true);
		ListUnpopulatedDeadEnd.Emplace(CurrentNode);
		
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(0, y * CellSize, 0.0f), FVector::OneVector));

		// Generate the wall of a node
		GenerateNodeWalls(CurrentNode, CurrentWallIndex);

		// Add the node in the maze map
		MazeMap.Emplace(CurrentNode);

		// Set initial wall display for the path (Hide or not)
		ISMWallComponent->UpdateInstanceTransform(CurrentNode.WallArray[int(EDirection::WEST)], FTransform(FQuat::Identity, FVector(100.0f + CurrentNode.Position.X * CellSize, CurrentNode.Position.Y * CellSize, -300), FVector::OneVector));

		for (int x = 1; x < Width - 1; x++) {
			CurrentNode = FNode(FVector2D(x, y), FVector2D(1, 0));

			ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector(x * CellSize, y * CellSize, 0.0f), FVector::OneVector));

			// Generate the wall of a node
			GenerateNodeWalls(CurrentNode, CurrentWallIndex);

			MazeMap.Emplace(CurrentNode);
			MazeMap[x + y * Width].SetLinkNbOthers(1);

			// Set initial wall display for the path (Hide or not)
			HideWallByDir(CurrentNode, EDirection::WEST);
		}

		CurrentNode = FNode(FVector2D(Width - 1, y), FVector2D(0, 1));
		ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, FVector((Width - 1) * CellSize, y * CellSize, 0.0f), FVector::OneVector));

		// Generate the wall of a node
		GenerateNodeWalls(CurrentNode, CurrentWallIndex);

		MazeMap.Emplace(CurrentNode);
		if (y != 0)
			MazeMap[Width-1 + y * Width].SetLinkNbOthers(2);

		// Set initial wall display for the path (Hide or not)
		if (y != (Height - 1))
			HideWallByDir(CurrentNode, EDirection::NORTH);
	}

	// Set Origin direction and position
	MazeMap[(Width - 1) + (Height - 1) * Width].SetDirection(FVector2D(0, 0));
	Origin = FVector2D(Width - 1, Height - 1);

	// Test Sphere to show origin
	SphereMesh->SetRelativeLocation(FVector((Width - 1) * CellSize, (Height - 1) * CellSize, 0));	
}

// One step iteration of the maze generation
void AMazeGenerator::MazeGenIteration()
{
	// Select a random direction
	int Index = FMath::RandRange(0, PossibleDirection.Num() - 1);
	FVector2D direction = PossibleDirection[Index];

	// Check if the next origin is in bound
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

	// Update the list of unpopulated dead end and 
	// if necessary the orientation of the items/door of the dead end
	UpdateNodeForDeadEnd(OriginNode);
	UpdateNodeForDeadEnd(NextNode);
	UpdateNodeForDeadEnd(TargetNextNode);

	// Swap Items and Doors in the dead ends
	if (NextNode->HasItem && !NextNode->isDeadEnd) {
		if (!TargetNextNode->HasItem && TargetNextNode->isDeadEnd) {
			TargetNextNode->TransferItemDoor(NextNode);

			TargetNextNode->UpdateTransformItem(TargetNextNode->LinkDirection, CellSize);
		
			ListUnpopulatedDeadEnd.Remove(*TargetNextNode);
			ListPopulatedDeadEnd.Emplace(*TargetNextNode);
		}
		else {
			FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
			ListUnpopulatedDeadEnd.Remove(RandNode);

			FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

			PtrRandNode->TransferItemDoor(NextNode);

			PtrRandNode->UpdateTransformItem(PtrRandNode->LinkDirection, CellSize);

			ListPopulatedDeadEnd.Emplace(*PtrRandNode);			
		}

		NextNode->ClearItemDoor();

		ListPopulatedDeadEnd.Remove(*NextNode);
	}

	if (OriginNode->HasItem && !OriginNode->isDeadEnd) {
		if (!NextNode->HasItem && NextNode->isDeadEnd) {
			NextNode->TransferItemDoor(OriginNode);

			NextNode->UpdateTransformItem(OriginNode->LinkDirection, CellSize);

			ListUnpopulatedDeadEnd.Remove(*NextNode);
			ListPopulatedDeadEnd.Emplace(*NextNode);
		}
		else if (!TargetNextNode->HasItem && TargetNextNode->isDeadEnd) {
			TargetNextNode->TransferItemDoor(OriginNode);

			TargetNextNode->UpdateTransformItem(TargetNextNode->LinkDirection, CellSize);
		
			ListUnpopulatedDeadEnd.Remove(*TargetNextNode);
			ListPopulatedDeadEnd.Emplace(*TargetNextNode);
		}
		else {
			FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
			ListUnpopulatedDeadEnd.Remove(RandNode);

			FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

			PtrRandNode->TransferItemDoor(OriginNode);

			PtrRandNode->UpdateTransformItem(PtrRandNode->LinkDirection, CellSize);

			ListPopulatedDeadEnd.Emplace(*PtrRandNode);
		}

		OriginNode->ClearItemDoor();

		ListPopulatedDeadEnd.Remove(*OriginNode);
	}

	// Test Sphere Move
	SphereMesh->SetRelativeLocation(FVector(Origin.X * CellSize, Origin.Y * CellSize, 0));
}

// Fully randomize the Maze for the beginning
void AMazeGenerator::MazeGenMultiStepRandomize()
{
	for (int i = 0; i < Width * Height * 10; i++)
		MazeGenIteration();
}

// Make the Maze evolve
void AMazeGenerator::MazeGenEvolution()
{
	for (int i = 0; i < Width * Height; i++)
		MazeGenIteration();
}

// Spawn Start and End
void AMazeGenerator::SpawnStartEnd()
{
	int MaxOuterWall = 2 * (Width + Height) - 1;

	int IndexStart = 0;
	int IndexEnd = 0;

	bool ValidPos = false;

	FTransform WallTransform;
	FVector Position;
	FNode Node;

	// Create the Start Room
	while (!ValidPos) {
		IndexStart = FMath::RandRange(0, MaxOuterWall);

		ISMWallComponent->GetInstanceTransform(IndexStart, WallTransform);

		Position = WallTransform.GetLocation();
		Position = FVector(FMath::Clamp(Position.X, 0, (Width-1) * CellSize - (CellSize * 0.5f)), FMath::Clamp(Position.Y, 0, (Height-1) * CellSize - (CellSize * 0.5f)), 0);
		Position /= CellSize;

		Node = MazeMap[Position.X + Position.Y * Width];

		if (Node.Door) ValidPos = true;
	}

	FVector StartDoorPos = WallTransform.GetLocation();
	FQuat StartDoorRot = WallTransform.GetRotation();

	// Hide the wall
	ISMWallComponent->UpdateInstanceTransform(IndexStart, FTransform(StartDoorRot, FVector(StartDoorPos.X, StartDoorPos.Y, -300), FVector::OneVector));

	// Create the Start Door
	ADoorObject* StartDoor = GetWorld()->SpawnActor<ADoorObject>(DoorBP, FTransform(StartDoorRot * FQuat(FVector::UpVector, -PI * 0.5f), StartDoorPos, FVector::OneVector));
	StartDoor->SetRequireKey(1);
	StartDoor->SetTier(EKeyDoorTier::KeyDoor_Common);
	ListObjects.Emplace(StartDoor);

	// Create the Start Room
	ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, StartDoorPos + StartDoorRot * FVector(CellSize, 0, 0), 2 * FVector::OneVector));
	ISMWallComponent->AddInstance(FTransform(StartDoorRot, StartDoorPos + StartDoorRot * FVector(2 * CellSize, 0, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(StartDoorRot * FQuat(FVector::UpVector, PI * 0.5f), StartDoorPos + StartDoorRot * FVector(CellSize, -CellSize, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(StartDoorRot * FQuat(FVector::UpVector, -PI * 0.5f), StartDoorPos + StartDoorRot * FVector(CellSize, CellSize, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(StartDoorRot * FQuat(FVector::UpVector, PI), StartDoorPos + StartDoorRot * FVector(0, CellSize, 0), FVector::OneVector));
	ISMWallComponent->AddInstance(FTransform(StartDoorRot * FQuat(FVector::UpVector, PI), StartDoorPos + StartDoorRot * FVector(0, -CellSize, 0), FVector::OneVector));

	// Set Player Starting Transform
	StartTransform = FTransform(StartDoorRot, StartDoorPos + StartDoorRot * FVector(CellSize, 0, 90), FVector::OneVector);

	// Add a key in the starting room
	AKeyItem* StartKey = GetWorld()->SpawnActor<AKeyItem>(KeyBP, FTransform(StartDoorRot * FQuat(FVector::UpVector, PI * 0.5f), StartDoorPos + StartDoorRot * FVector(CellSize * 0.5f, 0, 0), FVector::OneVector));
	ListObjects.Emplace(StartKey);

	// Create the End Room
	ValidPos = false;
	while (!ValidPos) {
		IndexEnd = FMath::RandRange(0, MaxOuterWall);

		if (FMath::Abs(IndexEnd-IndexStart) <= 1) continue;

		ISMWallComponent->GetInstanceTransform(IndexEnd, WallTransform);

		Position = WallTransform.GetLocation();
		Position = FVector(FMath::Clamp(Position.X, 0, (Width - 1) * CellSize - (CellSize * 0.5f)), FMath::Clamp(Position.Y, 0, (Height - 1) * CellSize - (CellSize * 0.5f)), 0);
		Position /= CellSize;

		Node = MazeMap[Position.X + Position.Y * Width];

		if (!Node.Door) ValidPos = true;
	}

	FVector EndDoorPos = WallTransform.GetLocation();
	FQuat EndDoorRot = WallTransform.GetRotation();
	
	// Hide the wall
	ISMWallComponent->UpdateInstanceTransform(IndexEnd, FTransform(EndDoorRot, FVector(EndDoorPos.X, EndDoorPos.Y, -300), FVector::OneVector));

	// Create the Exit Door
	ADoorObject* EndDoor = GetWorld()->SpawnActor<ADoorObject>(DoorBP, FTransform(EndDoorRot * FQuat(FVector::UpVector, PI * 0.5f), EndDoorPos, FVector::OneVector));
	EndDoor->SetRequireKey(NumberOfRareKey);
	EndDoor->SetTier(EKeyDoorTier::KeyDoor_Rare);

	// Create the End Room
	ISMFloorComponent->AddInstance(FTransform(FQuat::Identity, EndDoorPos + EndDoorRot * FVector(CellSize, 0, 0), 2 * FVector::OneVector));
	ISMWallComponent->AddInstance(FTransform(EndDoorRot, EndDoorPos + EndDoorRot * FVector(2 * CellSize, 0, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(EndDoorRot * FQuat(FVector::UpVector, PI * 0.5f), EndDoorPos + EndDoorRot * FVector(CellSize, -CellSize, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(EndDoorRot * FQuat(FVector::UpVector, PI * 0.5f), EndDoorPos + EndDoorRot * FVector(CellSize, CellSize, 0), FVector(1, 1.925f, 1)));
	ISMWallComponent->AddInstance(FTransform(EndDoorRot * FQuat(FVector::UpVector, PI), EndDoorPos + EndDoorRot * FVector(0, CellSize, 0), FVector::OneVector));
	ISMWallComponent->AddInstance(FTransform(EndDoorRot * FQuat(FVector::UpVector, PI), EndDoorPos + EndDoorRot * FVector(0, -CellSize, 0), FVector::OneVector));
}

// Spawn the specified number of MonsterAI
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

// Clear all the MonsterAI
void AMazeGenerator::MonsterAIClear()
{
	if (ListMonsters.IsEmpty()) return;

	for (AMonsterAI* monster : ListMonsters) {
		monster->Destroy();
	}

	ListMonsters.Empty();
}

// Spawn the trigger spikes
void AMazeGenerator::TriggerSpikesSpawn()
{
	TriggerSpikesClear();

	for (int i = 0; i < MazeMap.Num(); i++)
	{
		FNode* Node = &MazeMap[i];

		if (Node->isDeadEnd || FMath::SRand() > ProbaTriggerSpikes) continue;

		ATriggerSpikes* Spike = GetWorld()->SpawnActor<ATriggerSpikes>(TriggerSpikesBP, FTransform(GetActorRotation(), GetActorLocation() + FVector(Node->Position.X * CellSize, Node->Position.Y * CellSize, 40), FVector::OneVector));
		ListTriggerSpikes.Emplace(Spike);
		
		Node->Spike = Spike;
	}
}

// Clear all the trigger spikes
void AMazeGenerator::TriggerSpikesClear()
{
	if (ListTriggerSpikes.IsEmpty()) return;

	for (ATriggerSpikes* spikes : ListTriggerSpikes) {
		spikes->Destroy();
	}

	ListTriggerSpikes.Empty();
}

// Spawn the specified number of Rare Key
void AMazeGenerator::SpawnKeyDoor()
{
	ClearObjects();

	int NewKeyNumber = ListNumberKeyByTier[int(EKeyDoorTier::KeyDoor_Rare)];
	for (int i = 0; i < NewKeyNumber; i++) {
		ListNumberKeyByTier[int(EKeyDoorTier::KeyDoor_Rare)]--;
		KeySpawn(EKeyDoorTier::KeyDoor_Rare);
	}
}

// test clear objects 
void AMazeGenerator::ClearObjects()
{
	if (ListObjects.IsEmpty()) return;

	for (int i = 0; i < ListObjects.Num(); i++) {
		ListObjects[i]->Destroy();
	}

	ListObjects.Empty();
}

// Spawn the Monster AI in the maze
void AMazeGenerator::KeySpawn(EKeyDoorTier Tier)
{
	// Pick a random node from the unpopulated dead end
	FNode RandNode = ListUnpopulatedDeadEnd[FMath::RandRange(0, ListUnpopulatedDeadEnd.Num() - 1)];
	ListUnpopulatedDeadEnd.Remove(RandNode);

	FNode* PtrRandNode = &MazeMap[RandNode.Position.X + RandNode.Position.Y * Width];

	PtrRandNode->HasItem = true;

	// Create and setup the key
	AKeyItem* key = GetWorld()->SpawnActor<AKeyItem>(KeyBP);
	key->SetTier(Tier);
	key->UpdateKeyMesh();
	PtrRandNode->Item = key;

	// Create and setup the door
	ADoorObject* door = nullptr;

	switch (Tier)
	{
	case EKeyDoorTier::KeyDoor_Uncommon:
		door = GetWorld()->SpawnActor<ADoorObject>(DoorBP);
		door->SetTier(EKeyDoorTier::KeyDoor_Common);
		PtrRandNode->Door = door;
		break;
	case EKeyDoorTier::KeyDoor_Rare:
		door = GetWorld()->SpawnActor<ADoorObject>(DoorBP);
		door->SetTier(EKeyDoorTier::KeyDoor_Uncommon);
		PtrRandNode->Door = door;
		break;
	default:
		break;
	}

	// Update the direction of the item and door
	FVector2D DoorDir = FindDirDeadEnd(PtrRandNode);
	PtrRandNode->UpdateTransformItem(DoorDir, CellSize);

	ListObjects.Emplace(key);
	ListPopulatedDeadEnd.Emplace(*PtrRandNode);
	
	// If there is a door, create subsequent key and door of lower tier
	if (door) {
		ListObjects.Emplace(door);

		// Determine the number of key required for the current door (might need to rework that)
		int NewKeyNumber = FMath::RandRange(1, FMath::Max(1, int((ListNumberKeyByTier[int(door->DoorTier)] - ListNumberKeyByTier[int(Tier)]) * 0.6f)));

		if (!ListNumberKeyByTier[int(Tier)])
			NewKeyNumber = ListNumberKeyByTier[int(door->DoorTier)];

		door->SetRequireKey(NewKeyNumber);

		for (int i = 0; i < NewKeyNumber; i++) {
			ListNumberKeyByTier[int(door->DoorTier)]--;
			KeySpawn(door->DoorTier);
		}

	}
}

// Called to Spawn the Player at the Start Room
void AMazeGenerator::SpawnPlayer()
{
	ATheMazeCharacter* player = Cast<ATheMazeCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!player) return;

	player->SetActorRelativeTransform(StartTransform);
}

// Display Dead End
void AMazeGenerator::DisplayDeadEnd()
{
	CubeInstance->ClearInstances();

	for (FNode node : ListUnpopulatedDeadEnd) {
		CubeInstance->AddInstance(FTransform(FQuat::Identity, FVector(node.Position.X * CellSize, node.Position.Y * CellSize, 0), FVector::OneVector));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyItem.h"

// Sets default values
AKeyItem::AKeyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyTier = EKeyDoorTier::KeyDoor_Common;

	KeyItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	KeyItem->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeDefault(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeDefault.Succeeded()) {
		KeyItem->SetStaticMesh(CubeDefault.Object);
		KeyItem->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.8f));
		KeyItem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> CommonMat(TEXT("/Game/Materials/M_KeyCommon.M_KeyCommon"));
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> UncommonMat(TEXT("/Game/Materials/M_KeyUncommon.M_KeyUncommon"));
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> RareMat(TEXT("/Game/Materials/M_KeyRare.M_KeyRare"));
		
		switch (KeyTier)
		{
		case EKeyDoorTier::KeyDoor_Common:
			KeyItem->SetMaterial(0, CommonMat.Object);
			break;
		case EKeyDoorTier::KeyDoor_Uncommon:
			KeyItem->SetMaterial(0, UncommonMat.Object);
			break;
		case EKeyDoorTier::KeyDoor_Rare:
			KeyItem->SetMaterial(0, RareMat.Object);
			break;
		default:
			break;
		}
	}

}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Implement the Interact function of the interface
void AKeyItem::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "KEY RETRIEVED");

	bool success = player->FullHealCharacter();

	Destroy(true);
}
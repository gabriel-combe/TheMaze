// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyItem.h"

// Sets default values
AKeyItem::AKeyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyTier = EKeyDoorTier::KeyDoor_Common;

	KeyItemComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	KeyItemComp->SetupAttachment(RootComponent);
	KeyItemComp->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	
	KeyItemCommon = CreateDefaultSubobject<UStaticMesh>("KeyCommon");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyCommon(TEXT("/Game/Assets/Key_Common.Key_Common"));
	if (KeyCommon.Succeeded()) {
		KeyItemCommon = KeyCommon.Object;
	}

	KeyItemUncommon = CreateDefaultSubobject<UStaticMesh>("KeyUncommon");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyUncommon(TEXT("/Game/Assets/Key_Uncommon.Key_Uncommon"));
	if (KeyUncommon.Succeeded()) {
		KeyItemUncommon = KeyUncommon.Object;
	}

	KeyItemRare = CreateDefaultSubobject<UStaticMesh>("KeyRare");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyRare(TEXT("/Game/Assets/Key_Rare.Key_Rare"));
	if (KeyRare.Succeeded()) {
		KeyItemRare = KeyRare.Object;
	}

	KeyItemComp->SetStaticMesh(KeyItemCommon);
	KeyItemComp->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	KeyItemComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();

	UpdateKeyMesh();
}

// Called every frame
void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR

void AKeyItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AKeyItem, KeyTier))
	{
		UpdateKeyMesh();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Implement the Interact function of the interface
void AKeyItem::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "KEY RETRIEVED");

	player->AddKeyByType(KeyTier, 1);

	Destroy(true);
}

// Set the key tier
void AKeyItem::SetTier(EKeyDoorTier keyTier)
{
	KeyTier = keyTier;

	UpdateKeyMesh();
}

// Update the key mesh based on the key tier
void AKeyItem::UpdateKeyMesh()
{
	switch (KeyTier)
	{
	case EKeyDoorTier::KeyDoor_Common:
		KeyItemComp->SetStaticMesh(KeyItemCommon);
		break;
	case EKeyDoorTier::KeyDoor_Uncommon:
		KeyItemComp->SetStaticMesh(KeyItemUncommon);
		break;
	case EKeyDoorTier::KeyDoor_Rare:
		KeyItemComp->SetStaticMesh(KeyItemRare);
		break;
	default:
		break;
	}
}
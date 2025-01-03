// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorObject.h"

// Sets default values
ADoorObject::ADoorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorTier = EKeyDoorTier::KeyDoor_Common;
	RequiredKey = 5;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	DoorLComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorL"));
	DoorLComp->SetupAttachment(RootComponent);

	DoorRComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorR"));
	DoorRComp->SetupAttachment(RootComponent);

	DoorTopComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorTop"));
	DoorTopComp->SetupAttachment(RootComponent);

	DoorLComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); 
	DoorRComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	DoorTopComp->SetRelativeLocation(FVector(0.0f, 0.0f, 212.5f));
	DoorTopComp->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));


	WidgetDoor = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetDoor"));
	WidgetDoor->SetupAttachment(RootComponent);

	/*if (DoorInfoClass)
		WidgetDoor->SetWidgetClass(DoorInfoClass);*/

	WidgetDoor->SetWidgetSpace(EWidgetSpace::World);
	WidgetDoor->SetRelativeLocation(FVector(10.0f, 15.0f, 100.0f));
	WidgetDoor->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
	//WidgetDoor->SetVisibility(true);

	//DoorInfo = Cast<UDoorInfoWidget>(WidgetDoor->GetUserWidgetObject());
}

// Called when the game starts or when spawned
void ADoorObject::BeginPlay()
{
	Super::BeginPlay();

	/*if (DoorInfoClass)
		WidgetDoor->SetWidgetClass(DoorInfoClass);*/

	//WidgetDoor->InitWidget();

	//DoorInfo = Cast<UDoorInfoWidget>(WidgetDoor->GetUserWidgetObject());

	//UpdateDoorText();
}

// Called every frame
void ADoorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate Doors
	if (Open) {
		FQuat newRotL = FMath::Lerp(FQuat(DoorLComp->GetRelativeRotation()), FQuat(FRotator(0.0f, 90.0f, 0.0f)), 0.05f);
		DoorLComp->SetRelativeRotation(newRotL);
		DoorLComp->SetRelativeLocation(newRotL.RotateVector(FVector(92.0f, 0.0f, 0.0f)) + FVector(-92.0f, 0.0f, 0.0f));

		FQuat newRotR = FMath::Lerp(FQuat(DoorRComp->GetRelativeRotation()), FQuat(FRotator(0.0f, -90.0f, 0.0f)), 0.05f);
		DoorRComp->SetRelativeRotation(newRotR);
		DoorRComp->SetRelativeLocation(newRotR.RotateVector(FVector(-92.0f, 0.0f, 0.0f)) + FVector(92.0f, 0.0f, 0.0f));
	}

}

#if WITH_EDITOR

void ADoorObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ADoorObject, DoorTier))
	{
		UpdateDoorText();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Implement the Interact function of the interface
void ADoorObject::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "Try to open Door");

	RequiredKey = player->RemoveKeyByType(DoorTier, RequiredKey);
	
	if (RequiredKey == 0)
	{
		Open = true;
		WidgetDoor->SetVisibility(false);
		DoorLComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		DoorRComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "DOOR OPENED");
	}
}

// Set the key tier
void ADoorObject::SetTier(EKeyDoorTier doorTier)
{
	DoorTier = doorTier;
	UpdateDoorText();
}

// Set the required key number
void ADoorObject::SetRequireKey(int NbKey)
{
	RequiredKey = NbKey;
	UpdateDoorText();
}

// Update the text of the door
void ADoorObject::UpdateDoorText()
{
	WidgetDoor->RequestRenderUpdate();
	WidgetDoor->RequestRedraw();
	/*if (!DoorInfo) return;

	DoorInfo->SetText(FString::FromInt(RequiredKey));

	switch (DoorTier)
	{
	case EKeyDoorTier::KeyDoor_Common:
		DoorInfo->SetColour(FColor(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case EKeyDoorTier::KeyDoor_Uncommon:
		DoorInfo->SetColour(FColor(0.019608f, 0.423529f, 0.117647f, 1.0f));
		break;
	case EKeyDoorTier::KeyDoor_Rare:
		DoorInfo->SetColour(FColor(0.52549f, 0.039216f, 0.023529f, 1.0f));
		break;
	default:
		break;
	}*/
}
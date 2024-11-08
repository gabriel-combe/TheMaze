// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorObject.h"

// Sets default values
ADoorObject::ADoorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorTier = EKeyDoorTier::KeyDoor_Common;
	RequiredKey = 1;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	DoorLComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorL"));
	DoorLComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorL(TEXT("/Game/Assets/Door_Metal_L.Door_Metal_L"));
	if (DoorL.Succeeded()) {
		DoorLComp->SetStaticMesh(DoorL.Object);
	}

	DoorRComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorR"));
	DoorRComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorR(TEXT("/Game/Assets/Door_Metal_R.Door_Metal_R"));
	if (DoorR.Succeeded()) {
		DoorRComp->SetStaticMesh(DoorR.Object);
	}

	DoorLComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	DoorRComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	WidgetDoor = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetDoor"));

	/*static ConstructorHelpers::FObjectFinder<TSubclassOf<UUserWidget>> widget(TEXT("WidgetBlueprint'/Game/HUD/W_DoorText.W_DoorText'"));
	if (widget.Succeeded()) {
		WidgetDoor->SetWidgetClass(*widget.Object);
	}*/

	WidgetDoor->SetWidgetSpace(EWidgetSpace::World);
	WidgetDoor->SetRelativeLocation(FVector(0.0f, 1.0f, 0.0f));
	WidgetDoor->SetupAttachment(RootComponent);
	WidgetDoor->SetVisibility(true);
	
}

// Called when the game starts or when spawned
void ADoorObject::BeginPlay()
{
	Super::BeginPlay();

	//WidgetDoor->InitWidget();
}

// Called every frame
void ADoorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Open) {
		FQuat newRotL = FMath::Lerp(FQuat(DoorLComp->GetRelativeRotation()), FQuat(FRotator(0.0f, 90.0f, 0.0f)), 0.01f);
		DoorLComp->SetRelativeRotation(newRotL);
		DoorLComp->SetRelativeLocation(newRotL.RotateVector(FVector(92.0f, 0.0f, 0.0f)) + FVector(-92.0f, 0.0f, 0.0f));

		FQuat newRotR = FMath::Lerp(FQuat(DoorRComp->GetRelativeRotation()), FQuat(FRotator(0.0f, -90.0f, 0.0f)), 0.01f);
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
		//UpdateDoorMesh();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Implement the Interact function of the interface
void ADoorObject::Interact_Implementation(ATheMazeCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "DOOR OPENED");

	RequiredKey = player->RemoveKeyByType(DoorTier, RequiredKey);
	
	if (RequiredKey == 0)
		Open = true;
}

// Set the key tier
void ADoorObject::SetTier(EKeyDoorTier doorTier)
{
	DoorTier = doorTier;
}
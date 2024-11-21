// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerSpikes.h"

#include "TheMazeCharacter.h"


ATriggerSpikes::ATriggerSpikes()
{

	BoxCollisionComponent = CastChecked<UBoxComponent>(GetCollisionComponent());

	SetActorHiddenInGame(false);
	BoxCollisionComponent->bHiddenInGame = true;

	SpikesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spikes"));
	SpikesMesh->SetupAttachment(BoxCollisionComponent);
	SpikesMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SpikesMesh->SetGenerateOverlapEvents(false);
	SpikesMesh->bHiddenInGame = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeDefault(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeDefault.Succeeded()) {
		SpikesMesh->SetStaticMesh(ConeDefault.Object);
		SpikesMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		SpikesMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	damage = FMath::Max(damage, 0.0f);
}

void ATriggerSpikes::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Register Events
	OnActorBeginOverlap.AddDynamic(this, &ATriggerSpikes::DamagePlayer);
	OnActorEndOverlap.AddDynamic(this, &ATriggerSpikes::ResetTimer);
}

void ATriggerSpikes::DamagePlayer(AActor* OverlappedComponent, AActor* OtherActor)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "BEGIN OVERLAP");
	if (!OtherActor->ActorHasTag("Player")) return;

	ATheMazeCharacter* player = Cast<ATheMazeCharacter>(OtherActor);

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATriggerSpikes::ApplyDamage, player);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, true, 0.0f);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "BEGIN OVERLAP");
}

void ATriggerSpikes::ResetTimer(AActor* OverlappedComponent, AActor* OtherActor)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "END OVERLAP");
}

void ATriggerSpikes::ApplyDamage(class ATheMazeCharacter* player)
{
	player->DamageCharacter(damage);
}
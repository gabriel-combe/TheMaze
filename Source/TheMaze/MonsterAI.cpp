// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"
#include "Components/CapsuleComponent.h"
#include "TheMazeCharacter.h"

// Sets default values
AMonsterAI::AMonsterAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"));
	if (RightFistCollisionBox) {
		RightFistCollisionBox->SetBoxExtent(FVector(15.f, 5.f, 5.f), false);
		RightFistCollisionBox->SetupAttachment(GetMesh(), FName("hand_r_socket"));
		RightFistCollisionBox->SetRelativeLocation(FVector(-10.f, 0, 0));
	}


	LeftFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistCollisionBox"));
	if (LeftFistCollisionBox) {
		LeftFistCollisionBox->SetBoxExtent(FVector(15.f, 5.f, 5.f), false);
		LeftFistCollisionBox->SetupAttachment(GetMesh(), FName("hand_l_socket"));
		LeftFistCollisionBox->SetRelativeLocation(FVector(10.f, 0, 0));
	}

	// Set Default Monster tag
	Tags.Add("Monster");
}

// Called when the game starts or when spawned
void AMonsterAI::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AMonsterAI::Stun);
}

// Called every frame
void AMonsterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterAI::SetBlackboard(UBlackboardComponent* b)
{
	Blackboard = b;
}

// Called to bind functionality to input
void AMonsterAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonsterAI::Stun(AActor* OverlappedComponent, AActor* OtherActor)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	ATheMazeCharacter* player = Cast<ATheMazeCharacter>(OtherActor);

	if (!player->IsInvincible()) return;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandleStun);

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMonsterAI::ResetStun);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleStun, Delegate, 2.0f, false);

	Blackboard->SetValueAsBool("IsStunned", true);
}

void AMonsterAI::ResetStun()
{
	Blackboard->SetValueAsBool("IsStunned", false);
}
// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheMazeCharacter.h"
#include "TheMazeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "Interactable.h"
#include "MazeData.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATheMazeCharacter

ATheMazeCharacter::ATheMazeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Set Health to max Health
	currentHealth = maxHealth;

	// Set Ability Points to max Ability Points
	currentAbilityPoints = maxAbilityPoints;

	// Set the number of Key to 0
	keyCount.Init(0, StaticEnum<EKeyDoorTier>()->GetMaxEnumValue());

	// Set Dead to false
	dead = false;

	// Set default chrono value
	ChronoTime = 60.0f;
}

void ATheMazeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Set Default Player tag
	Tags.Add("Player");

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATheMazeCharacter::SetDead);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleChrono, Delegate, ChronoTime, false);
}

//////////////////////////////////////////////////////////////////////////// Input

void ATheMazeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheMazeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheMazeCharacter::Look);
		
		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATheMazeCharacter::Interact);

		// Using
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &ATheMazeCharacter::Use);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATheMazeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATheMazeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATheMazeCharacter::Interact(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Interact");

	FHitResult outHit;

	FCollisionShape boxTrace = FCollisionShape::MakeBox(FVector3f(40.0f, 40.0f, 90.0f));
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * traceDistance;

	if (!GetWorld()->SweepSingleByChannel(outHit, Start, End, FQuat::Identity, ECC_Visibility, boxTrace)) return;

	AActor* object = outHit.GetActor();
	if (!object->GetClass()->ImplementsInterface(UInteractable::StaticClass())) return;

	IInteractable::Execute_Interact(object, this);

	// TEST START

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 2.0f);

	HealCharacter(10.0f);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%f"), currentHealth));
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("Hit actor is: %s"), *outHit.GetActor()->GetName()));
	}
	// TEST END
 
}

void ATheMazeCharacter::Use(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Use");

	// TEST START
	RemoveAbilityPoint();

	DamageCharacter(15.0f);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), dead));
	// TEST END
}

bool ATheMazeCharacter::HealCharacter(const float HealAmount)
{
	if (currentAbilityPoints >= maxHealth) return false;

	currentHealth += HealAmount;

	currentHealth = FMath::Clamp(currentHealth, 0.0f, maxHealth);

	return true;
}

bool ATheMazeCharacter::DamageCharacter(const float DamageAmount)
{
	currentHealth -= DamageAmount;

	if (currentHealth <= 0.0f) {
		dead = true;
		return false;
	}

	currentHealth = FMath::Clamp(currentHealth, 0.0f, maxHealth);

	return true;
}

bool ATheMazeCharacter::FullHealCharacter()
{
	if (currentHealth == maxHealth) return false;

	currentHealth = maxHealth;

	return true;
}

void ATheMazeCharacter::AddKeyByType(EKeyDoorTier keyType, int number)
{
	keyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))] += number;
}

int ATheMazeCharacter::RemoveKeyByType(EKeyDoorTier keyType, int number)
{
	int count = keyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))];
	int rest = FMath::Max(number - count, 0);

	keyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))] -= FMath::Min(count, number);

	return rest;
}

bool ATheMazeCharacter::RemoveAbilityPoint()
{
	if (currentAbilityPoints <= 0) return false;

	FTimerHandle TimerHandleAbility;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATheMazeCharacter::RecoverAbilityPoint);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAbility, Delegate, 5.0f, false);
	
	currentAbilityPoints--;

	return true;
}

void ATheMazeCharacter::RecoverAbilityPoint()
{
	currentAbilityPoints++;
}
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
	CurrentHealth = MaxHealth;

	// Set Ability Points to max Ability Points
	CurrentAbilityPoints = MaxAbilityPoints;

	// Set the number of Key to 0
	KeyCount.Init(0, StaticEnum<EKeyDoorTier>()->GetMaxEnumValue());

	// Save the base max walk speed
	SpeedBase = GetCharacterMovement()->MaxWalkSpeed;
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
	FVector End = Start + GetActorForwardVector() * TraceDistance;

	if (!GetWorld()->SweepSingleByChannel(outHit, Start, End, FQuat::Identity, ECC_Visibility, boxTrace)) return;

	AActor* object = outHit.GetActor();
	if (!object->GetClass()->ImplementsInterface(UInteractable::StaticClass())) return;

	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("Hit actor is: %s"), *object->GetName()));

	IInteractable::Execute_Interact(object, this);

	// TEST START

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 2.0f);

	HealCharacter(10.0f);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%f"), CurrentHealth));
	// TEST END
 
}

void ATheMazeCharacter::Use(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Use");

	bool success = RemoveAbilityPoint();

	if (!success) return;

	DashCharacter();

	SetInvincibility();

	GiveSpeedBoost();	
}

bool ATheMazeCharacter::HealCharacter(const float HealAmount)
{
	if (CurrentAbilityPoints >= MaxHealth) return false;

	CurrentHealth += HealAmount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	return true;
}

bool ATheMazeCharacter::DamageCharacter(const float DamageAmount)
{
	if (Invincible || Dead) return false;

	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0.0f) {
		SetDead();
		return false;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	return true;
}

bool ATheMazeCharacter::FullHealCharacter()
{
	if (CurrentHealth == MaxHealth) return false;

	CurrentHealth = MaxHealth;

	return true;
}

void ATheMazeCharacter::AddKeyByType(EKeyDoorTier keyType, int number)
{
	KeyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))] += number;
}

int ATheMazeCharacter::RemoveKeyByType(EKeyDoorTier keyType, int number)
{
	int count = KeyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))];
	int rest = FMath::Max(number - count, 0);

	KeyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))] -= FMath::Min(count, number);

	return rest;
}

bool ATheMazeCharacter::RemoveAbilityPoint()
{
	if (CurrentAbilityPoints <= 0) return false;

	// Create a timer to reset the ability point
	FTimerHandle TimerHandleAbility;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATheMazeCharacter::RecoverAbilityPoint);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleAbility, Delegate, APRecoverDuration, false);
	
	CurrentAbilityPoints--;

	return true;
}

void ATheMazeCharacter::RecoverAbilityPoint()
{
	CurrentAbilityPoints++;
}

void ATheMazeCharacter::DashCharacter()
{
	FVector XYDir = GetVelocity() * (FVector::ForwardVector + FVector::RightVector);

	if (XYDir.Length() < 0.01f)
		XYDir = GetActorForwardVector();

	XYDir.Normalize();

	LaunchCharacter(XYDir * DashDistance, true, true);
}

void ATheMazeCharacter::SetInvincibility()
{
	// Clear any unfinished timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleInvincibility);

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATheMazeCharacter::ResetInvincibility);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleInvincibility, Delegate, InvincibilityDuration, false);

	Invincible = true;
}

void ATheMazeCharacter::ResetInvincibility()
{
	Invincible = false;
}

void ATheMazeCharacter::GiveSpeedBoost()
{
	// Clear any unfinished timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleSpeedBoost);
	GetCharacterMovement()->MaxWalkSpeed = SpeedBase;

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ATheMazeCharacter::ResetSpeedBoost);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleSpeedBoost, Delegate, SpeedBoostDuration, false);

	GetCharacterMovement()->MaxWalkSpeed = SpeedBase * SpeedBoostMultiplier;
}

void ATheMazeCharacter::ResetSpeedBoost()
{
	GetCharacterMovement()->MaxWalkSpeed = SpeedBase;
}
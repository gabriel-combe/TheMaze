// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TheMazeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/** Delegate for when the player dies **/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATheMazeCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Current Health */
	float currentHealth;

	/** Current Ability Points */
	int currentAbilityPoints;
	
	/** Number of Key owned */
	TArray<int> keyCount;

	/** Whether the character is dead or not */
	bool dead;

	/** Game timer before death **/
	FTimerHandle TimerHandleChrono;

	/** Triggered when the player dies **/
	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FPlayerIsDead OnPlayerDied;

	/** Time for the chrono **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Chrono", meta = (AllowPrivateAccess = "true"))
	float ChronoTime;

	/** Distance of the box trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true", ClampMin = 0.0f))
	float traceDistance = 100.0f;

	/** Maximum Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Health", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float maxHealth = 100.0f;

	/** Maximum Ability Points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int maxAbilityPoints = 3;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;
	
public:
	ATheMazeCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);

	/** Called for use input */
	void Use(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns the percentage of health remaining **/
	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	float GetPercentHealth() const { return currentHealth / maxHealth;  }

	/** Returns the current ability points **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	int GetCurrentAbilityPoints() const { return currentAbilityPoints; }

	/** Returns the current number of key owned **/
	UFUNCTION(BlueprintCallable, Category = "Player|Key")
	int GetKeyCountByType(EKeyDoorTier keyType) const { return keyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))]; }

	/** Get Remaining time in the chrono **/
	UFUNCTION(BlueprintCallable, Category = "Player|Chrono")
	float GetRemainingTimePercent() const { return GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandleChrono) / ChronoTime; };

	/** Heal Character and returns whether it was successful or not **/
	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	bool HealCharacter(const float HealAmount);

	/** Damage Character and returns whether it was successful or not **/
	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	bool DamageCharacter(const float DamageAmount);

	/** Full heal the Character **/
	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	bool FullHealCharacter();

	/** Add keys in the array of key by its type **/
	UFUNCTION(BlueprintCallable, Category = "Player|Key")
	void AddKeyByType(EKeyDoorTier keyType, int number);

	/** Remove keys in the array of key by its type **/
	UFUNCTION(BlueprintCallable, Category = "Player|Key")
	int RemoveKeyByType(EKeyDoorTier keyType, int number);

	/** Remove an ability point **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	bool RemoveAbilityPoint();

	/** Recover an ability point **/
	void RecoverAbilityPoint();

	/** Set player dead **/
	UFUNCTION(BlueprintCallable, Category = "Player|State")
	void SetDead() {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "UR DEAD");

		dead = true;
	};
};

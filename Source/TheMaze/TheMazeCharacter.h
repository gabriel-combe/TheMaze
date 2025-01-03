// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MazeDataEnum.h"
#include "MazeHUD.h"
#include "MazeGameInstance.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
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
	
private:
	/** Current Health */
	float CurrentHealth;

	/** Current Ability Points */
	int CurrentAbilityPoints;
	
	/** Number of Key owned */
	TArray<int> KeyCount;

	/** Whether the character is dead or not */
	bool IsDead = false;

	/** whether the character is invincible or not */
	bool Invincible = false;

	/** Base speed of the Player */
	float SpeedBase;

	/** Speed boost for the dash */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float SpeedBoostMultiplier = 2.0f;

	/** Speed boost duration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float SpeedBoostDuration = 5.0f;

	/** Invincibility Duration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float InvincibilityDuration = 2.0f;

	/** Ability Point Recover duration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float APRecoverDuration = 5.0f;

	/** Game timer before death **/
	FTimerHandle TimerHandleChrono;

	/** Invincibility Timer **/
	FTimerHandle TimerHandleInvincibility;

	/** Speed Boost Timer **/
	FTimerHandle TimerHandleSpeedBoost;

	/** Triggered when the player dies **/
	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FPlayerIsDead OnPlayerDied;

	/** HUD used by the player **/
	TObjectPtr<AMazeHUD> PlayerMazeHUD;

	/** Time for the chrono **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Chrono", meta = (AllowPrivateAccess = "true"))
	float ChronoTime = 60.0f;

	/** Distance of the box trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true", ClampMin = 0.0f))
	float TraceDistance = 150.0f;

	/** Maximum Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Health", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float MaxHealth = 100.0f;

	/** Maximum Ability Points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int MaxAbilityPoints = 3;

	/** Dash power */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ability", meta = (AllowPrivateAccess = "true"))
	float DashPower = 10000.0f;

	// Box collision for the head
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> HeadCollisionBox;

	/** Maze game instance */
	TObjectPtr<UMazeGameInstance> MazeGI;

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

	/** Use Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	// Stimulus source of the player used by the monster ai
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	// Setup of the stimulus source
	void SetupStimulusSource();

protected:
	virtual void BeginPlay();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);

	/** Called for use input */
	void Use(const FInputActionValue& Value);

	/** Called for pause input */
	void Pause(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Called when there is an overlap with the head of the player */
	UFUNCTION()
	void Catched(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	ATheMazeCharacter();
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns the percentage of health remaining **/
	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	float GetPercentHealth() const { return CurrentHealth / MaxHealth;  }

	/** Returns the current ability points **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	int GetCurrentAbilityPoints() const { return CurrentAbilityPoints; }

	/** Returns the current number of key owned **/
	UFUNCTION(BlueprintCallable, Category = "Player|Key")
	int GetKeyCountByType(EKeyDoorTier keyType) const { return KeyCount[StaticEnum<EKeyDoorTier>()->GetIndexByValue(int64(keyType))]; }

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

	/** Make the player dash in its movement direction or looking direction if standing still **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	void DashCharacter();

	/** Activate Invincibility **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	void SetInvincibility();

	/** Activate Invincibility **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	bool IsInvincible() const { return Invincible; };

	/** Deactivate invincibility **/
	void ResetInvincibility();

	/** Activate Speed Boost **/
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	void GiveSpeedBoost();

	/** Deactivate Speed Boost **/
	void ResetSpeedBoost();

	/** Set player dead **/
	UFUNCTION(BlueprintCallable, Category = "Player|State")
	void SetDead();

	/** Set player win **/
	UFUNCTION(BlueprintCallable, Category = "Player|State")
	void SetWin();
};

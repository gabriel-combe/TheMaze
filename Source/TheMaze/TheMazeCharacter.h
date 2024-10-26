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
	int keyCount;

	/** Distance of the box trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true", ClampMin = 0.0f))
	float traceDistance = 100.0f;

	/** Maximum Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true", ClampMin = 1.0f))
	float maxHealth = 100.0f;

	/** Maximum Ability Points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true", ClampMin = 1))
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
	UFUNCTION(BlueprintCallable, Category = "TheMaze")
	float GetPercentHealth() const { return currentHealth / maxHealth;  }

	/** Returns the current ability points **/
	UFUNCTION(BlueprintCallable, Category = "TheMaze")
	int GetCurrentAbilityPoints() const { return currentAbilityPoints; }

	/** Returns the current number of key owned **/
	UFUNCTION(BlueprintCallable, Category = "TheMaze")
	int GetKeyCount() const { return keyCount; }

};


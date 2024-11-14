// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "DoorInfoWidget.generated.h"


/**
 * 
 */
UCLASS()
class THEMAZE_API UDoorInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Initialize all the Component of our widget
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> MultiplyImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> KeyImage;

	// Set Door Text
	UFUNCTION(BlueprintCallable)
	void SetText(FString text);

	// Set Door Text and Image Colour
	UFUNCTION(BlueprintCallable)
	void SetColour(FColor colour);
};

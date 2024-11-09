// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInfoWidget.h"


void UDoorInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set icons textures
	/*const FString tempMultPath = "/Game/Textures/multiply-icon.multiply-icon";
	const FString tempKeyPath = "/Game/Textures/key.key";

	MultTex = LoadObject<UTexture2D>(this, *tempMultPath);
	KeyTex = LoadObject<UTexture2D>(this, *tempKeyPath);*/


	TObjectPtr<UCanvasPanel> Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	//HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName("HorizontalBox"));
	KeyNumber = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("KeyNumber"));
	//MultiplyImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName("MultiplyImage"));
	//KeyImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName("KeyImage"));


	KeyNumber->SetText(FText::FromString(TEXT("KeyNumber")));

	//MultiplyImage->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(MultTex));
	//MultiplyImage->SetBrushFromTexture(MultTex);
	//MultiplyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	
	//KeyImage->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(KeyTex));
	//KeyImage->SetBrushFromTexture(KeyTex);
	//KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	Canvas->AddChild(KeyNumber);
	//Canvas->AddChild(MultiplyImage);
	//Canvas->AddChild(KeyImage);
	/*HorizontalBox->AddChildToHorizontalBox(KeyNumber);
	HorizontalBox->AddChildToHorizontalBox(MultiplyImage);
	HorizontalBox->AddChildToHorizontalBox(KeyImage);*/

	TObjectPtr<UCanvasPanelSlot> KeyTextSlot = Cast<UCanvasPanelSlot>(KeyNumber->Slot);
	if (KeyTextSlot) {
		KeyTextSlot->SetAutoSize(true);
		KeyTextSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		KeyTextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	}

	/*TObjectPtr<UCanvasPanelSlot> MultImgSlot = Cast<UCanvasPanelSlot>(MultiplyImage->Slot);
	if (MultImgSlot) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("WTF"));

		MultImgSlot->SetAutoSize(true);
		MultImgSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	}

	TObjectPtr<UCanvasPanelSlot> KeyImgSlot = Cast<UCanvasPanelSlot>(KeyImage->Slot);
	if (KeyImgSlot) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("AKIRA"));

		KeyImgSlot->SetAutoSize(true);
		KeyImgSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	}*/


	WidgetTree->RootWidget = Canvas;
}

void UDoorInfoWidget::SetText(FString text)
{
	KeyNumber->SetText(FText::FromString(text));
}

void UDoorInfoWidget::SetTextColour(FColor colour)
{
	KeyNumber->SetColorAndOpacity(FSlateColor(colour));
}
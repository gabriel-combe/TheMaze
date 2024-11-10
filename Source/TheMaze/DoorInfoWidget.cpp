// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInfoWidget.h"


void UDoorInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyNumber->SetText(FText::FromString(TEXT("1")));
	MultiplyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UDoorInfoWidget::SetText(FString text)
{
	//KeyNumber->SetText(FText::FromString(text));
	if (KeyNumber) {
		KeyNumber->SetText(FText::FromString(FString("WTF")));

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "SETTING TEXT");
	}
}

void UDoorInfoWidget::SetColour(FColor colour)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, "SETTING COLOUR");

	if (KeyNumber)
		KeyNumber->SetColorAndOpacity(FSlateColor(FColor(0.52549f, 0.039216f, 0.023529f, 1.0f)));
	
	if (MultiplyImage)
		MultiplyImage->SetColorAndOpacity(FLinearColor(colour));
	
	if (KeyImage)
		KeyImage->SetColorAndOpacity(FLinearColor(colour));
}
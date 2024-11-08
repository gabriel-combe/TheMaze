// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInfoWidget.h"

void UDoorInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (KeyNumber)
		KeyNumber->SetText(FText::FromString(TEXT("KeyNumber")));

	if (MultiplyImage)
		MultiplyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	if (KeyImage)
		KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
}
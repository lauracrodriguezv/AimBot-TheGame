// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_PromptWidget.h"
#include "Components/TextBlock.h"

void UBPE_PromptWidget::SetPromptInformation(const FString& Text)
{
	if(IsValid((InformationText)))
	{
		InformationText->SetText(FText::FromString(Text));
	}
}

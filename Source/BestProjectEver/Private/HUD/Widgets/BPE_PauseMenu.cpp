// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_PauseMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBPE_PauseMenu::SetPauseMenuInformation(bool bWasPausedByOwner)
{
	if(IsValid(PauseTittleText))
	{
		const FString Text = bWasPausedByOwner ? "Game paused" : "Paused by other player" ;
		PauseTittleText->SetText(FText::FromString(Text));
	}

	if(IsValid(ResumeButton))
	{
		const ESlateVisibility ResumeButtonVisibility = bWasPausedByOwner ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		ResumeButton->SetVisibility(ResumeButtonVisibility);
	}
}

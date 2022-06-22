// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_PauseMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/BPE_PlayerController.h"

void UBPE_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
	ResumeButton->OnClicked.AddDynamic(this, &UBPE_PauseMenu::ResumeGame);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::SetPauseMenuInformation(bool BWasPauseRequestByOwner)
{
	if(IsValid(PauseTittleText))
	{
		const FString Text = BWasPauseRequestByOwner ? "Game paused" : "Paused by other player";
		PauseTittleText->SetText(FText::FromString(Text));
	}

	if(IsValid(ResumeButton))
	{
		const ESlateVisibility ResumeButtonVisibility = BWasPauseRequestByOwner ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		ResumeButton->SetVisibility(ResumeButtonVisibility);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::ResumeGame()
{
	ABPE_PlayerController* PlayerController = Cast<ABPE_PlayerController>(GetOwningPlayer());
	if(IsValid(PlayerController))
	{
		PlayerController->SetGamePause(false);	
	}
}
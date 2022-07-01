// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_PauseMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/BPE_PlayerController.h"

void UBPE_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
	PlayerControllerReference = Cast<ABPE_PlayerController>(GetOwningPlayer());
	InitializeButtons();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::SetPauseMenuInformation(bool bWasPauseRequestByOwner)
{
	if(IsValid(PauseTittleText))
	{
		const FString Text = bWasPauseRequestByOwner ? "Game paused" : "Paused by other player";
		PauseTittleText->SetText(FText::FromString(Text));
	}

	const ESlateVisibility ButtonsVisibility = bWasPauseRequestByOwner ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	if(IsValid(ResumeButton))
	{
		ResumeButton->SetVisibility(ButtonsVisibility);
	}
	
	if(IsValid(ShowroomButton) && IsValid(PlayerControllerReference) && PlayerControllerReference->HasAuthority())
	{
		ShowroomButton->SetVisibility(ButtonsVisibility);
	}
	
	if(IsValid(MainMenuButton))
	{
		MainMenuButton->SetVisibility(ButtonsVisibility);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::ResumeGame()
{
	if(IsValid(PlayerControllerReference))
	{
		PlayerControllerReference->SetGamePause(false);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::InitializeButtons()
{
	if(IsValid(ResumeButton))
	{
		ResumeButton->OnClicked.AddDynamic(this, &UBPE_PauseMenu::ResumeGame);	
	}
	
	if(IsValid(ShowroomButton))
	{
		if(IsValid(PlayerControllerReference) && PlayerControllerReference->HasAuthority())
		{
			ShowroomButton->OnClicked.AddDynamic(this, &UBPE_PauseMenu::TravelToShowroom);
		}
		else
		{
			ShowroomButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UBPE_PauseMenu::TravelToMainMenu);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::TravelToShowroom()
{
	if(IsValid(PlayerControllerReference))
	{
		PlayerControllerReference->Server_TravelToMap("/Game/Maps/Lobby?listen");	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_PauseMenu::TravelToMainMenu()
{
	if(IsValid(PlayerControllerReference))
	{
		if(PlayerControllerReference->HasAuthority())
		{
			PlayerControllerReference->Server_TravelToMap("/Game/Maps/GameStartupMap?listen");
		}
		else
		{
			ResumeGame();
			PlayerControllerReference->ClientTravel("/Game/Maps/GameStartupMap", TRAVEL_Absolute, true);
		}
	}
}

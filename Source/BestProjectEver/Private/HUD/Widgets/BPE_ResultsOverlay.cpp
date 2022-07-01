// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_ResultsOverlay.h"

#include "Components/Button.h"
#include "HUD/Widgets/BPE_TimerWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "PlayerController/BPE_PlayerController.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerControllerReference = Cast<ABPE_PlayerController>(GetOwningPlayer());
	InitializeButtons();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::InitializeButtons()
{
	if(IsValid(ShowroomButton))
	{
		if(IsValid(PlayerControllerReference) && PlayerControllerReference->HasAuthority())
		{
			ShowroomButton->OnClicked.AddDynamic(this, &UBPE_ResultsOverlay::TravelToShowroom);
		}
		else
		{
			ShowroomButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UBPE_ResultsOverlay::TravelToMainMenu);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::TravelToShowroom()
{
	if(IsValid(PlayerControllerReference))
	{
		PlayerControllerReference->Server_TravelToMap("/Game/Maps/Lobby?listen");	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::TravelToMainMenu()
{
	if(IsValid(PlayerControllerReference))
	{
		if(PlayerControllerReference->HasAuthority())
		{
			PlayerControllerReference->Server_TravelToMap("/Game/Maps/GameStartupMap?listen");
		}
		else
		{
			PlayerControllerReference->ClientTravel("/Game/Maps/GameStartupMap", TRAVEL_Absolute, true);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::UpdateMatchTimer(const float TimeLeft)
{
	if(IsValid(W_Timer))
	{
		W_Timer->SetTime(TimeLeft);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::ShowMatchResult(EMatchResult MatchResult)
{
	const FString Text = MatchResult == EMatchResult::Victory ? "You Win!!!" : "You Lose.";
	if(IsValid((InformationText)))
	{
		InformationText->SetText(FText::FromString(Text));	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::ShowTopScoringPlayers(const TArray<APlayerState*>& TopScoringPlayers)
{
	FString Text;
	
	if(TopScoringPlayers.Num() == 1 && IsValid(GetOwningPlayer()) && TopScoringPlayers[0] == GetOwningPlayer()->GetPlayerState<APlayerState>())
	{
		Text = FString("You are the top scoring player!!!");
	}
	else if(TopScoringPlayers.Num() == 1 )
	{
		Text = FString::Printf(TEXT("Top scoring player is: %s"),  *TopScoringPlayers[0]->GetPlayerName());
	}
	else if (TopScoringPlayers.Num() > 1)
	{
		Text = FString("You both get the same score!!");
	}
	else if(TopScoringPlayers.IsEmpty())
	{
		Text = FString("You didn't get the top score :(");
	}

	if(IsValid(TopScoringPlayersText))
	{
		TopScoringPlayersText->SetText(FText::FromString(Text));	
	}
}

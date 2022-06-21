// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_ResultsOverlay.h"
#include "HUD/Widgets/BPE_TimerWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::UpdateMatchTimer(const float TimeLeft)
{
	if(IsValid(W_Timer))
	{
		W_Timer->SetTime(TimeLeft);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::SetMatchResultText(EMatchResult MatchResult)
{
	const FString Text = MatchResult == EMatchResult::Victory ? "You Win!!!" : "You Lose.";
	if(IsValid((InformationText)))
	{
		InformationText->SetText(FText::FromString(Text));	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::SetTopScoringPlayersText(const TArray<APlayerState*> TopScoringPlayers, const APlayerState* PlayerStateReference)
{
	FString Text;
	
	if(TopScoringPlayers.Num() == 1 && TopScoringPlayers[0] == PlayerStateReference)
	{
		Text = FString("You are the top scoring player!!!");
	}
	else if(TopScoringPlayers.Num() == 1 )
	{
		Text = FString::Printf(TEXT("Top scoring player is: \n%s"),  *TopScoringPlayers[0]->GetPlayerName());
	}
	else if (TopScoringPlayers.Num() > 1)
	{
		Text = FString("Players with top score: \n");
		for (const APlayerState* PlayerState : TopScoringPlayers)
		{
			Text.Append(FString::Printf(TEXT("%s\n"), *PlayerState->GetPlayerName()));
		}
	}

	if(IsValid(TopScoringPlayersText))
	{
		TopScoringPlayersText->SetText(FText::FromString(Text));	
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerState/BPE_PlayerState.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_PlayerState::ABPE_PlayerState()
{
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	OnScoreIncreased();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerState::OnScoreIncreased()
{
	OnScoreChanged.Broadcast(GetScore());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerState::IncreaseScore(const float ScoreAmount)
{
	float CurrentScore = GetScore();
	SetScore(CurrentScore += ScoreAmount);
	
	OnScoreIncreased();
}

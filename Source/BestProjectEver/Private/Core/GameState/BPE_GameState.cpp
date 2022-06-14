// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameState/BPE_GameState.h"

#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Math/UnitConversion.h"
#include "Net/UnrealNetwork.h"

ABPE_GameState::ABPE_GameState()
{
	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::OnRep_TimeLeft()
{
	OnTimeLeftUpdated.Broadcast(TimeLeft);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::SetTimeLeft(float Time)
{
	TimeLeft = Time;
	OnTimeLeftUpdated.Broadcast(TimeLeft);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABPE_GameState, TimeLeft);
}


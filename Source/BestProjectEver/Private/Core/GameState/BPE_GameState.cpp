// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameState/BPE_GameState.h"

#include "Character/BPE_Enemy.h"
#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "Net/UnrealNetwork.h"

ABPE_GameState::ABPE_GameState()
{
	bAreAllEnemiesDead = false;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::BeginPlay()
{
	Super::BeginPlay();
	SetEnemiesOnMatch();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::SetEnemiesOnMatch()
{
	if(HasAuthority())
	{
		TArray<AActor*> EnemiesInGame;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABPE_Enemy::StaticClass(), EnemiesInGame);
		EnemiesAlive = EnemiesInGame.Num();	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::OnRep_TimeLeft()
{
	OnTimeLeftUpdated.Broadcast(TimeLeft);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();
	OnMatchStateSet.Broadcast(GetMatchState());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::SetTimeLeft(float Time)
{
	TimeLeft = Time;
	OnTimeLeftUpdated.Broadcast(TimeLeft);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::DecreaseEnemiesAlive()
{
	if(HasAuthority())
	{
		--EnemiesAlive;
		OnEnemyDeath.Broadcast(EnemiesAlive);
		if(EnemiesAlive <= 0)
		{
			bAreAllEnemiesDead = true;
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::OnRep_EnemiesAlive()
{
	OnEnemyDeath.Broadcast(EnemiesAlive);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABPE_GameState, TimeLeft);
	DOREPLIFETIME(ABPE_GameState, EnemiesAlive);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameState/BPE_GameState.h"

#include "Character/BPE_Enemy.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/BPE_PlayerController.h"

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
void ABPE_GameState::Multicast_SetPause_Implementation(bool bPause, APlayerState* InstigatedBy)
{
	OnSetPause.Broadcast(bPause);

	if(bPause)
	{
		PlayersWhoPaused.AddUnique(InstigatedBy);	
	}
	else
	{
		PlayersWhoPaused.Remove(InstigatedBy);
	}
	
	const float TimeDilation = !bPause? 1.0f : 0.000001f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);

	UpdatePlayerPauseState();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameState::UpdatePlayerPauseState()
{
	ABPE_PlayerController* PlayerController = Cast<ABPE_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(IsValid(PlayerController))
	{
		if(IsGamePaused())
		{
			if(IsValid(PlayerController->GetPlayerState<APlayerState>()))
			{
				const EPauseState PauseState = WasPauseInstigatedByPlayer(PlayerController->GetPlayerState<APlayerState>())?
					EPauseState::PausedBySelf : EPauseState::PausedByOtherPlayer;
			
				PlayerController->SetPauseState(PauseState);	
			}
		}
		else
		{
			PlayerController->SetPauseState(EPauseState::UnPause);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_GameState::WasPauseInstigatedByPlayer(const APlayerState* Player) const
{
	return PlayersWhoPaused.Contains(Player);
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


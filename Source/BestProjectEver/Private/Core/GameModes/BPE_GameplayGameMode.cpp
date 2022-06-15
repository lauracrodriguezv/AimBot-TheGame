// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_GameplayGameMode.h"

#include "AI/BPE_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BPE_PlayerCharacter.h"
#include "Character/BPE_Enemy.h"
#include "Core/GameState/BPE_GameState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}


//----------------------------------------------------------------------------------------------------------------------
ABPE_GameplayGameMode::ABPE_GameplayGameMode()
{
	bDelayedStart = true;
	
	MatchTime = 120.0f;
	WarmupTime = 10.0f;
	CooldownTime = 15.0f;
	LevelStartingTime = 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
	GetWorldTimerManager().SetTimer(TimerHandle_RefreshTimeLeftRate, this, &ABPE_GameplayGameMode::UpdateTimeLeft, 1.0f, true, 0.0f);	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::InitializeReferences()
{
	TimeLeft = WarmupTime;
	LevelStartingTime = GetWorld()->GetTimeSeconds();
	GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	if(TimeLeft <= 0.0f)
	{
		StartMatch();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::HandleMatchStarted()
{
	if(TimeLeft <= 0.0f)
	{
		StartCooldown();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::HandleCooldownTime()
{
	if(TimeLeft <= 0.0f)
	{
		GetWorld()->ServerTravel(FString("/Game/Maps/Gameplay?listen"));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::StartCooldown()
{
	SetMatchState(MatchState::Cooldown);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::HandlePlayerDeath(AController* KillerController, AController* KilledController,
	ABPE_PlayerCharacter* KilledCharacter)
{
	//if the killer was another player, set killer's ultimate xp to 0
	//show the time left to respawn
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::HandleEnemyDeath(AController* KillerController, AController* KilledController,
	ABPE_Enemy* KilledCharacter)
{
	// increase killer score and xp for ultimate
	// decrease total number of enemies alive
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::UpdateTimeLeft()
{
	if (MatchState == MatchState::WaitingToStart)
	{
		TimeLeft = WarmupTime - (GetWorld()->GetTimeSeconds() - LevelStartingTime);
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == MatchState::InProgress)
	{
		TimeLeft = (WarmupTime + MatchTime) - (GetWorld()->GetTimeSeconds() - LevelStartingTime);
		HandleMatchStarted();
	}
	else if(MatchState == MatchState::Cooldown)
	{
		TimeLeft = (WarmupTime + MatchTime + CooldownTime) - (GetWorld()->GetTimeSeconds() - LevelStartingTime);
		HandleCooldownTime();
	}

	if(IsValid(GameStateReference))
	{
		GameStateReference->SetTimeLeft(TimeLeft);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter)
{
	if(IsValid(KilledController))
	{
		KilledController->ChangeState(EName::Inactive);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::RespawnPlayer(AController* KilledController, APawn* KilledCharacter)
{
	if(IsValid(KilledCharacter))
	{
		KilledCharacter->Reset();
		KilledCharacter->Destroy();
	}
	
	if(IsValid(KilledController))
	{
		AActor* PlayerStart = FindPlayerStart(KilledController);
		RestartPlayerAtPlayerStart(KilledController, PlayerStart);
	}
}
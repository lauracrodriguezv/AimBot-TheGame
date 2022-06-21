// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_GameplayGameMode.h"

#include "AI/BPE_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BPE_PlayerCharacter.h"
#include "Character/BPE_Enemy.h"
#include "Core/GameState/BPE_GameState.h"
#include "Core/PlayerState/BPE_PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

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
	GameStateReference = Cast<ABPE_GameState>(GameState);
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
void ABPE_GameplayGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	if(TimeLeft <= 0.0f || IsValid(GameStateReference) && GameStateReference->AreAllEnemiesDead())
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
void ABPE_GameplayGameMode::StartMatch()
{
	TimeLeft = MatchTime;
	Super::StartMatch();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::StartCooldown()
{
	TimeLeft = CooldownTime;
	SetMatchState(MatchState::Cooldown);
	if(IsValid(GameStateReference))
	{
		GameStateReference->HandleMatchResults();	
	}
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
	if(IsValid(KillerController))
	{
		ABPE_PlayerState* PlayerState = Cast<ABPE_PlayerState>(KillerController->GetPlayerState<ABPE_PlayerState>());
		if(IsValid(PlayerState))
		{
			PlayerState->IncreaseScore(1.0);
		}
	}

	if(IsValid(GameStateReference))
	{
		GameStateReference->DecreaseEnemiesAlive();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::UpdateTimeLeft()
{
	--TimeLeft;
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if(MatchState == MatchState::Cooldown)
	{
		HandleCooldownTime();
	}

	if(IsValid(GameStateReference))
	{
		GameStateReference->SetTimeLeft(TimeLeft);
	}
}

//----------------------------------------------------------------------------------------------------------------------
AActor* ABPE_GameplayGameMode::ChoosePlayerStart_Implementation(AController* Player)
{	
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* Current = *It;
		if(Current->PlayerStartTag != "Taken")
		{
			Current->PlayerStartTag = "Taken";
			return Current;
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameplayGameMode::OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter)
{
	if(IsValid(KilledController))
	{
		KilledController->ChangeState(EName::Inactive);	
	}

	if(KilledCharacter->IsA(ABPE_Enemy::StaticClass()))
	{
		ABPE_Enemy* EnemyKilled = Cast<ABPE_Enemy>(KilledCharacter);
		HandleEnemyDeath(KillerController, KilledController, EnemyKilled);
	}
	else if(KilledCharacter->IsA(ABPE_PlayerCharacter::StaticClass()))
	{
		ABPE_PlayerCharacter* PlayerKilled = Cast<ABPE_PlayerCharacter>(KilledCharacter);
		HandlePlayerDeath(KillerController, KilledController, PlayerKilled);
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
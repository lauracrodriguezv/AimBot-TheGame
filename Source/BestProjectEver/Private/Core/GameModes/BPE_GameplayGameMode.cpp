// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_GameplayGameMode.h"

#include "AI/BPE_AIController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BPE_PlayerCharacter.h"
#include "Character/BPE_Enemy.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_GameplayGameMode::ABPE_GameplayGameMode()
{
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
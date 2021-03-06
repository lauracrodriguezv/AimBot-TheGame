// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "PlayerController/BPE_PlayerController.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_LobbyGameMode::ABPE_LobbyGameMode()
{
#if !WITH_EDITOR
	bUseSeamlessTravel = true;	
#endif
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_LobbyGameMode::TravelToMap(const FString& InURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	return GetWorld()->ServerTravel(InURL, bAbsolute, bShouldSkipGameNotify);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_LobbyGameMode::OnCharacterDeath(AController* KillerController, AController* KilledController,
	ABPE_BaseCharacter* KilledCharacter)
{
	if(IsValid(KilledController) && IsValid(Cast<ABPE_PlayerController>(KilledController)))
	{
		KilledController->ChangeState(EName::Inactive);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_LobbyGameMode::RespawnPlayer(AController* KilledController, APawn* KilledCharacter)
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


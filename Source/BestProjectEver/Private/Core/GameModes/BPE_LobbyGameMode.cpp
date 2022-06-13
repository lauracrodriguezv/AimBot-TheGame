// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_LobbyGameMode::ABPE_LobbyGameMode()
{
	bUseSeamlessTravel = true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_LobbyGameMode::TravelToMatchMap()
{
	if(IsValid(GameState))
	{
		const int32 NumberOfPlayers = GameState->PlayerArray.Num();
		if(NumberOfPlayers >= 2)
		{
			/** seamless travel to the match map open as a listen server for clients to connect to*/
			GetWorld()->ServerTravel(FString("/Game/Maps/Gameplay?listen"));
		}	
	}
}

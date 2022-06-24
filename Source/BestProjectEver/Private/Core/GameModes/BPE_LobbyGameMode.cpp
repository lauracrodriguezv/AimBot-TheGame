// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/BPE_LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

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


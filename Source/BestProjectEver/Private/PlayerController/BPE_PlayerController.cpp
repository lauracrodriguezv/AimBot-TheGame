// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BPE_PlayerController.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Core/GameModes/BPE_LobbyGameMode.h"
#include "Core/GameState/BPE_GameState.h"
#include "HUD/BPE_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_PlayerController::ABPE_PlayerController()
{
	TimeToRespawn = 1.0f;
	bAreGameplayInputsEnabled = true;

	PauseState = EPauseState::UnPause;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::SetPauseState(EPauseState NewPauseState)
{
	PauseState = NewPauseState;
	
	HUD = Cast<ABPE_HUD>(GetHUD());
	if(IsValid(HUD))
	{
		HUD->UpdatePauseMenu(PauseState);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	InitializeReferences();
#endif
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::InitializeReferences()
{
	if(!IsValid(GameStateReference))
	{
		GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
		if(IsValid(GameStateReference))
		{
			GameStateReference->OnMatchStateSet.AddDynamic(this, &ABPE_PlayerController::OnMatchStateChanged);
		}	
	}

	//SetPlayerName();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if !WITH_EDITOR
	InitializeReferences();
#endif
	
	if(HasAuthority())
	{
		/** This delay is created because GetHUD() is not valid at the first tick, that's why the HUD was not updated on client */
		GetWorldTimerManager().SetTimer(TimerHandle_HUD,this, &ABPE_PlayerController::Client_UpdateHud,
			0.5f,false, 0.5f);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::SetPlayerName()
{
	if(IsValid(GameStateReference) && IsValid(PlayerState))
	{
		PlayerState->SetPlayerName(FString("Player ") + FString::FromInt(GameStateReference->PlayerArray.IndexOfByKey(PlayerState) + 1));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::Server_TravelToMap_Implementation(const FString& URL)
{
	GetWorld()->ServerTravel(URL);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerController::Server_TravelToMap_Validate(const FString& URL)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::Client_UpdateHud_Implementation()
{
	HUD = Cast<ABPE_HUD>(GetHUD());
	if(IsValid(HUD))
	{
		HUD->AddCharacterOverlay();
		HUD->BindDelegates();
		HUD->AddPauseMenu();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if(IsValid(HUD))
	{
		HUD->RemoveCharacterOverlay();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::OnMatchStateChanged(const FName MatchState)
{
	if(MatchState == MatchState::Cooldown)
	{
		BeginCooldownState();
		bShowMouseCursor = true;
	}
}
	
//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::BeginInactiveState()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ABPE_PlayerController::RequestRespawn,
		TimeToRespawn, false, TimeToRespawn);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::BeginCooldownState()
{
	bAreGameplayInputsEnabled = false;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::RequestRespawn()
{
	if(IsValid(GetWorld()->GetAuthGameMode<ABPE_GameplayGameMode>()))
	{
		ABPE_GameplayGameMode* GameplayGameMode = GetWorld()->GetAuthGameMode<ABPE_GameplayGameMode>();
		GameplayGameMode->RespawnPlayer(this,GetPawn());
	}
	else if (IsValid(GetWorld()->GetAuthGameMode<ABPE_LobbyGameMode>()))
	{
		ABPE_LobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<ABPE_LobbyGameMode>();
		LobbyGameMode->RespawnPlayer(this,GetPawn());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::SetGamePause(bool bPause)
{
	if(HasAuthority())
	{
		if(IsValid(GameStateReference))
		{
			GameStateReference->Multicast_SetPause(bPause, PlayerState);
		}
	}
	else
	{
		Server_SetMatchPause(bPause);
	}
	
	bShowMouseCursor = bPause;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::Server_SetMatchPause_Implementation(bool bPause)
{
	SetGamePause(bPause);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerController::Server_SetMatchPause_Validate(bool bPause)
{
	return true;
}


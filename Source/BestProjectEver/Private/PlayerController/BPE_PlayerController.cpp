// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BPE_PlayerController.h"

#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Core/GameState/BPE_GameState.h"
#include "HUD/BPE_HUD.h"
#include "GameFramework/PlayerState.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_PlayerController::ABPE_PlayerController()
{
	TimeToRespawn = 1.0f;
	bAreGameplayInputsEnabled = true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ABPE_GameState* GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
	if(IsValid(GameStateReference))
	{
		GameStateReference->OnMatchStateSet.AddDynamic(this, &ABPE_PlayerController::OnMatchStateChanged);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetPlayerName();
	
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
	const ABPE_GameState* GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
	if(IsValid(GameStateReference) && IsValid(PlayerState))
	{
		PlayerState->SetPlayerName(FString("Player ") + FString::FromInt(GameStateReference->PlayerArray.IndexOfByKey(PlayerState) + 1));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::Server_TravelToMap_Implementation(const FString& FURL)
{
	GetWorld()->ServerTravel(FURL);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerController::Server_TravelToMap_Validate(const FString& FURL)
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
	ABPE_GameplayGameMode* GameplayGameMode = GetWorld()->GetAuthGameMode<ABPE_GameplayGameMode>();
	if(IsValid(GameplayGameMode))
	{
		GameplayGameMode->RespawnPlayer(this,GetPawn());
	}
}
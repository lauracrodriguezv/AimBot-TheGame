// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BPE_PlayerController.h"

#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "HUD/BPE_HUD.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_PlayerController::ABPE_PlayerController()
{
	TimeToRespawn = 1.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorldTimerManager().SetTimer(TimerHandle_HUD,this, &ABPE_PlayerController::Client_UpdateHud,
		0.3f,false, 0.3f);
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
bool ABPE_PlayerController::Client_UpdateHud_Validate()
{
	return true;
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
void ABPE_PlayerController::BeginInactiveState()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ABPE_PlayerController::RequestRespawn,
		TimeToRespawn, false, TimeToRespawn);
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ABP_PlayerController.h"

#include "HUD/BPE_HUD.h"

void AABP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<ABPE_HUD>(GetHUD());
	if(IsValid(PlayerHUD))
	{
		PlayerHUD->AddCharacterOverlay();
	}
}

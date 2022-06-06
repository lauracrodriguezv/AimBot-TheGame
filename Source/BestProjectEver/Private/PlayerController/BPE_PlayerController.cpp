// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BPE_PlayerController.h"

#include "HUD/BPE_HUD.h"

void ABPE_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ABPE_HUD>(GetHUD());
	if(IsValid(HUD))
	{
		HUD->AddCharacterOverlay();
	}
}

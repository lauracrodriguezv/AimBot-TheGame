// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BPE_HUD.h"
#include "HUD/Widgets/BPE_PlayerHUD.h"
#include "GameFramework/PlayerController.h"

void ABPE_HUD::AddCharacterOverlay()
{
	const APlayerController* PlayerController = GetOwningPlayerController();
	if(IsValid(PlayerController) && IsValid(CharacterOverlayClass))
	{
		
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BPE_HUD.h"

#include "Character/BPE_PlayerCharacter.h"
#include "HUD/Widgets/BPE_CharacterOverlay.h"
#include "GameFramework/PlayerController.h"

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	AddCharacterOverlay();
	InitiliazeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::InitiliazeReferences()
{
	APawn* PlayerPawn = GetOwningPawn();
	if(	IsValid(PlayerPawn))
	{
		ABPE_PlayerCharacter* PlayerCharacter = Cast<ABPE_PlayerCharacter>(PlayerPawn);
		if(IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnChangeCurrentWeapon.AddDynamic(this, &ABPE_HUD::UpdateCurrentWeapon);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::DrawHUD()
{
	Super::DrawHUD();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(IsValid(PlayerController) && IsValid(CharacterOverlayClass))
	{
		CharacterOverlay = CreateWidget<UBPE_CharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::UpdateCurrentWeapon(EColorType WeaponColorType)
{
	if(IsValid(CharacterOverlay))
	{
		CharacterOverlay->UpdateWeaponInfo(WeaponColorType);	
	}
}

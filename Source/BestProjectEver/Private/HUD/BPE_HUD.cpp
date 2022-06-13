// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BPE_HUD.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/BPE_HealthComponent.h"
#include "HUD/Widgets/BPE_CharacterOverlay.h"
#include "GameFramework/PlayerController.h"

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::BindDelegates()
{
	ABPE_PlayerCharacter* PlayerCharacter = Cast<ABPE_PlayerCharacter>( GetOwningPawn());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnChangeCurrentWeaponDelegate.AddDynamic(this, &ABPE_HUD::UpdateCurrentWeaponIcon);
		PlayerCharacter->GetHealthComponent()->OnHealthChangeDelegate.AddDynamic(this, &ABPE_HUD::UpdateHealth);
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
void ABPE_HUD::RemoveCharacterOverlay()
{
	if(IsValid(CharacterOverlay))
	{
		CharacterOverlay->RemoveFromParent();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::UpdateCurrentWeaponIcon(const EColorType WeaponColorType)
{
	if(IsValid(CharacterOverlay))
	{
		CharacterOverlay->UpdateWeaponIcons(WeaponColorType);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::UpdateHealth(const FHealthData& HealthData)
{
	if(IsValid(CharacterOverlay))
	{
		CharacterOverlay->UpdateHealthDisplay(HealthData.CurrentHealth, HealthData.MaxHealth);	
	}
}

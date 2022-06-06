// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_CharacterOverlay.h"

#include "Components/Image.h"

void UBPE_CharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWeaponIcons();	
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateWeaponInfo(EColorType WeaponColorType)
{
	if(IconsWeapon.Contains(WeaponColorType))
	{
		SetIconRenderParameters(IconsWeapon[WeaponColorType]);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::SetIconRenderParameters(UImage* Icon)
{
	if(IsValid(CurrentIconSelected))
	{
		CurrentIconSelected->SetRenderScale(NotSelectedSize);
	}

	if(IsValid(Icon))
	{
		CurrentIconSelected = Icon;
		CurrentIconSelected->SetRenderScale(SelectedSize);

		if(CurrentIconSelected->GetRenderOpacity() < 1.0)
		{
			CurrentIconSelected->SetRenderOpacity(1.0f);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UBPE_CharacterOverlay::InitializeWeaponIcons()
{
	IconsWeapon.Add(EColorType::Yellow, Yellow);
	IconsWeapon.Add(EColorType::Blue, Blue);
	IconsWeapon.Add(EColorType::Red, Red);
	
	SelectedSize = FVector2D(1.0f, 1.0f);
	NotSelectedSize = FVector2D(0.3f, 0.3f);
}


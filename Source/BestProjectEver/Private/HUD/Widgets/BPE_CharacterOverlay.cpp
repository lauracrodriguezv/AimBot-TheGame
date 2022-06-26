// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_CharacterOverlay.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/Widgets/BPE_TimerWidget.h"
#include "HUD/Widgets/BPE_CrosshairWidget.h"

void UBPE_CharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeWeaponIcons();
	WeaponIndex = 0;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateWeaponIcons(EColorType WeaponColorType)
{
	if(!WeaponIcons.Contains(WeaponColorType) && CurrentWeaponColor.Contains(WeaponColorType))
	{
		WeaponIndex = WeaponIndex < WeaponImages.Num()? WeaponIndex : 0;
		WeaponIcons.Add(WeaponColorType, FWeaponIcon(WeaponImages[WeaponIndex], CurrentWeaponColor[WeaponColorType]));
		++WeaponIndex;
	}
	
	SetIconRenderParameters(WeaponColorType);
	
	if(IsValid(W_Crosshair) && WeaponIcons.Contains(WeaponColorType))
	{
		W_Crosshair->SetCrosshairColor(WeaponIcons[WeaponColorType].IconColor);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateHealthDisplay(float Health, float MaxHealth)
{
	const float HealthPercent = Health / MaxHealth;
	HealthProgressBar->SetPercent(HealthPercent);
	
	FString Text = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
	HealthText->SetText(FText::FromString(Text));
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateUltimateDisplay(const float CurrentUltimateValue, const float MaxUltimateValue)
{
	const float UltimatePercent = CurrentUltimateValue / MaxUltimateValue;
	if(IsValid(UltimateProgressBar))
	{
		UltimateProgressBar->SetPercent(UltimatePercent);
	}
	
	const FString Text = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(CurrentUltimateValue), FMath::CeilToInt(MaxUltimateValue));
	if(IsValid(UltimateText))
	{
		UltimateText->SetText(FText::FromString(Text));	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateMatchTimer(const float TimeLeft)
{
	if(IsValid(W_Timer))
	{
		W_Timer->SetTime(TimeLeft);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateScoreText(const float Score)
{	
	const FString Text = FString::FromInt(Score);
	if(IsValid(ScoreText))
	{
		ScoreText->SetText(FText::FromString(Text));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateEnemiesAliveText(const int32 EnemiesAlive)
{
	const FString Text = FString::FromInt(EnemiesAlive);
	if(IsValid(EnemiesText))
	{
		EnemiesText->SetText(FText::FromString(Text));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::SetIconRenderParameters(EColorType WeaponColorType)
{
	if(WeaponIcons.Contains(WeaponColorType))
	{
		if(IsValid(CurrentIconSelected))
		{
			CurrentIconSelected->SetRenderScale(WeaponIcons[WeaponColorType].NotSelectedScale);
		}

		if(IsValid(WeaponIcons[WeaponColorType].Icon))
		{
			CurrentIconSelected = WeaponIcons[WeaponColorType].Icon;
			CurrentIconSelected->SetRenderScale(WeaponIcons[WeaponColorType].SelectedScale);
			CurrentIconSelected->SetColorAndOpacity(WeaponIcons[WeaponColorType].IconColor);
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::InitializeWeaponIcons()
{
	WeaponImages.AddUnique(LeftWeaponIcon);
	WeaponImages.AddUnique(CenterWeaponIcon);
	WeaponImages.AddUnique(RightWeaponIcon);
	
	CurrentWeaponColor.Add(EColorType::Yellow, FLinearColor::Yellow);
	CurrentWeaponColor.Add(EColorType::Blue, FLinearColor::Blue);
	CurrentWeaponColor.Add(EColorType::Red, FLinearColor::Red);
}


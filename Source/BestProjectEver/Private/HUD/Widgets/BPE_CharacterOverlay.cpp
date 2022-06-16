// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_CharacterOverlay.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/Widgets/BPE_TimerWidget.h"

void UBPE_CharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeWeaponIcons();
	SelectedScale = FVector2D(2.0f, 2.0f);
	NotSelectedScale = FVector2D(0.8f, 0.8f);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateWeaponIcons(EColorType WeaponColorType)
{
	if(IconsWeapon.Contains(WeaponColorType))
	{
		SetIconRenderParameters(IconsWeapon[WeaponColorType]);
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
	const FString Text = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
	if(IsValid(ScoreText))
	{
		ScoreText->SetText(FText::FromString(Text));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::UpdateEnemiesAliveText(const int32 EnemiesAlive)
{
	const FString Text = FString::Printf(TEXT("%d"), EnemiesAlive);
	if(IsValid(EnemiesText))
	{
		EnemiesText->SetText(FText::FromString(Text));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::SetIconRenderParameters(UImage* Icon)
{
	if(IsValid(CurrentIconSelected))
	{
		CurrentIconSelected->SetRenderScale(NotSelectedScale);
	}

	if(IsValid(Icon))
	{
		CurrentIconSelected = Icon;
		CurrentIconSelected->SetRenderScale(SelectedScale);

		if(CurrentIconSelected->GetRenderOpacity() < 1.0)
		{
			CurrentIconSelected->SetRenderOpacity(1.0f);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_CharacterOverlay::InitializeWeaponIcons()
{
	IconsWeapon.Add(EColorType::Yellow, YellowWeapon);
	IconsWeapon.Add(EColorType::Blue, BlueWeapon);
	IconsWeapon.Add(EColorType::Red, RedWeapon);
}


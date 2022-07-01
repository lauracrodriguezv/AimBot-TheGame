// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UBPE_EnemyHealthBar::UpdateHealthDisplay(const FHealthData& HealthData)
{
	const float HealthPercent = HealthData.CurrentHealth / HealthData.MaxHealth;
	HealthProgressBar->SetPercent(HealthPercent);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_EnemyHealthBar::SetHealthBarColor(FLinearColor ProgressBarColor)
{
	HealthProgressBar->SetFillColorAndOpacity(ProgressBarColor);
}



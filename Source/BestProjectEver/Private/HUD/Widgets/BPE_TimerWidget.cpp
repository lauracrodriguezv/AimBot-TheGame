// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_TimerWidget.h"

#include "Components/TextBlock.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_TimerWidget::SetTime(const float Time)
{
	const int32 Minutes = FMath::FloorToInt(Time / 60.0f);
	const int32 Seconds = Time - Minutes * 60.0f;
			
	const FString MatchCountdown = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	if(IsValid(MatchCountdownText))
	{
		MatchCountdownText->SetText(FText::FromString(MatchCountdown));	
	}
}

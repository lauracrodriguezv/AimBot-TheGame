// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_ResultsOverlay.h"
#include "HUD/Widgets/BPE_TimerWidget.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_ResultsOverlay::UpdateMatchTimer(const float TimeLeft)
{
	if(IsValid(W_Timer))
	{
		W_Timer->SetTime(TimeLeft);
	}
}

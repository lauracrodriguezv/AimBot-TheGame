// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widgets/BPE_AnnouncementOverlay.h"
#include "HUD/Widgets/BPE_TimerWidget.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_AnnouncementOverlay::UpdateMatchTimer(const float TimeLeft)
{
	if(IsValid(W_Timer))
	{
		W_Timer->SetTime(TimeLeft);
	}
}

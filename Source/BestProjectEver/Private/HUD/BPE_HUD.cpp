// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BPE_HUD.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/BPE_HealthComponent.h"
#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Core/GameState/BPE_GameState.h"
#include "GameFramework/GameMode.h"
#include "HUD/Widgets/BPE_CharacterOverlay.h"
#include "GameFramework/PlayerController.h"
#include "HUD/Widgets/BPE_TimerWidget.h"
#include "HUD/Widgets/BPE_AnnouncementOverlay.h"
#include "HUD/Widgets/BPE_ResultsOverlay.h"

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::InitializeReferences()
{
	GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
	if(IsValid(GameStateReference))
	{
		GameStateReference->OnTimeLeftUpdated.AddDynamic(this, &ABPE_HUD::UpdateMatchTimeLeft);
		GameStateReference->OnMatchStateSet.AddDynamic(this, &ABPE_HUD::UpdateOverlay);
	}
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
	if(MatchState == MatchState::Cooldown)
	{
		return;
	}
	
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

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::UpdateMatchTimeLeft(const float TimeLeft)
{
	if (MatchState == MatchState::WaitingToStart)
	{
		if(IsValid(AnnouncementOverlay))
		{
			AnnouncementOverlay->UpdateMatchTimer(TimeLeft);
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		if(IsValid(CharacterOverlay))
		{
			CharacterOverlay->UpdateMatchTimer(TimeLeft);
		}
	}
	else if(MatchState == MatchState::Cooldown)
	{
		if(IsValid(ResultsOverlay))
		{
			ResultsOverlay->UpdateMatchTimer(TimeLeft);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::UpdateOverlay(const FName NewMatchState)
{
	MatchState = NewMatchState;
	if (MatchState == MatchState::WaitingToStart)
	{
		AddAnnouncementOverlay();
	}
	else if (MatchState == MatchState::InProgress)
	{
		RemoveAnnouncementOverlay();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		RemoveCharacterOverlay();
		AddResultsOverlay();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::AddAnnouncementOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(IsValid(PlayerController) && IsValid(AnnouncementOverlayClass))
	{
		AnnouncementOverlay = CreateWidget<UBPE_AnnouncementOverlay>(PlayerController, AnnouncementOverlayClass);
		AnnouncementOverlay->AddToViewport();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::RemoveAnnouncementOverlay()
{
	if(IsValid(AnnouncementOverlay))
	{
		AnnouncementOverlay->RemoveFromParent();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::AddResultsOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(IsValid(PlayerController) && IsValid(ResultsOverlayClass))
	{
		ResultsOverlay = CreateWidget<UBPE_ResultsOverlay>(PlayerController, ResultsOverlayClass);
		ResultsOverlay->AddToViewport();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HUD::RemoveResultsOverlay()
{
	if(IsValid(ResultsOverlay))
	{
		ResultsOverlay->RemoveFromParent();
	}
}

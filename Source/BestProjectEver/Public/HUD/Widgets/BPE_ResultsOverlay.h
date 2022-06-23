// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/MatchDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "BPE_ResultsOverlay.generated.h"

class UTextBlock;
class UBPE_TimerWidget;
class UButton;
class ABPE_PlayerController;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_ResultsOverlay : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Transient)
	TObjectPtr<ABPE_PlayerController> PlayerControllerReference;
	
	//------------------------------------------------------------------------------------------------------------------
	//Information

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TittleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InformationText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TopScoringPlayersText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ShowroomButton;
	
	//------------------------------------------------------------------------------------------------------------------
	//Match Time

	/** countdown on match in progress */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBPE_TimerWidget> W_Timer;
	
protected:

	virtual void NativeConstruct() override;

	/** Enable buttons depending on network role */
	void InitializeButtons();

	/** User server travel if has authority or client travel if it is a client */
	UFUNCTION()
	void TravelToShowroom();

	/** User server travel if has authority or client travel if it is a client */
	UFUNCTION()
	void TravelToMainMenu();

public:

	/** show the player the time left to end current match state */
	void UpdateMatchTimer(const float TimeLeft);

	void ShowMatchResult(EMatchResult MatchResult);

	void ShowTopScoringPlayers(const TArray<APlayerState*>& TopScoringPlayers);
};


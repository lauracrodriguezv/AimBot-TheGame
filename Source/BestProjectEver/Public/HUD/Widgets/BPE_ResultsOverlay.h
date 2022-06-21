// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/MatchDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "BPE_ResultsOverlay.generated.h"

class UTextBlock;
class UBPE_TimerWidget;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_ResultsOverlay : public UUserWidget
{
	GENERATED_BODY()

	//------------------------------------------------------------------------------------------------------------------
	//Information

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TittleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InformationText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TopScoringPlayersText;

	//------------------------------------------------------------------------------------------------------------------
	//Match Time

	/** countdown on match in progress */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBPE_TimerWidget> W_Timer;

public:

	/** show the player the time left to end current match state */
	void UpdateMatchTimer(const float TimeLeft);

	void SetMatchResultText(EMatchResult MatchResult);

	void SetTopScoringPlayersText(const TArray<APlayerState*> TopScoringPlayers, const APlayerState* PlayerStateReference);
};


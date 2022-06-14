// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPE_TimerWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_TimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	//------------------------------------------------------------------------------------------------------------------
	//Match Time

	/** this has to be the same name on the blueprint class */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MatchCountdownText;
	
public:

	/** show the player the time left to end current match state */
	void SetTime(const float Time);
};

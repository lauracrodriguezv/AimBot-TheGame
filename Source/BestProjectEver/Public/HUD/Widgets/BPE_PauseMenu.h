// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPE_PauseMenu.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PauseTittleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

public:

	void SetPauseMenuInformation(bool bWasPausedByOwner);
};

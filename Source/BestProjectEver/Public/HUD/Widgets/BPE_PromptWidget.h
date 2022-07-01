// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPE_PromptWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_PromptWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InformationText;

public:

	void SetPromptInformation(const FString& Text);
};

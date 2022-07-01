// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/HealthData.h"
#include "Blueprint/UserWidget.h"
#include "BPE_EnemyHealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

public:

	UFUNCTION()
	void UpdateHealthDisplay(const FHealthData& HealthData);

	void SetHealthBarColor(FLinearColor ProgressBarColor);
};

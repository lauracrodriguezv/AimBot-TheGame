// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BPE_CrosshairWidget.generated.h"

/**
 * Crosshair for shoot mechanics, updated depending on weapon type
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_CrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** color bind to the crosshair images */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	FLinearColor CrosshairColor;
	
public:

	void SetCrosshairColor(const FLinearColor Color) { CrosshairColor = Color; }
	
};

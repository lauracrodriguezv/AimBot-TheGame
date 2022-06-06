// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BestProjectEver/ColorType.h"
#include "BPE_CharacterOverlay.generated.h"

class UImage;

UCLASS()
class BESTPROJECTEVER_API UBPE_CharacterOverlay : public UUserWidget
{
	
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
	UImage* Yellow;

	UPROPERTY(meta = (BindWidget))
	UImage* Blue;

	UPROPERTY(meta = (BindWidget))
	UImage* Red;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	UImage* CurrentIconSelected;
		
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D SelectedSize;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D NotSelectedSize;

	TMap<EColorType, UImage*> IconsWeapon;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetIconRenderParameters(UImage* Icon);

	void InitializeWeaponIcons();

public:

	void UpdateWeaponInfo(EColorType WeaponColorType);
};
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

	//------------------------------------------------------------------------------------------------------------------
	//Weapon
	
	/** yellow weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* YellowWeapon;

	/** blue weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* BlueWeapon;

	/** red weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* RedWeapon;

	/** current weapon icon selected */
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	UImage* CurrentIconSelected;

	/** render scale when a weapon icon is selected */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D SelectedScale;

	/** render scale when a weapon icon is not selected, by default in the widget blueprint the icons are set to this scale */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D NotSelectedScale;

	/** map to access the icons with the color type of the weapon */
	TMap<EColorType, UImage*> IconsWeapon;

protected:

	virtual void NativeConstruct() override;

	/** [local] change icons render properties */
	UFUNCTION()
	void SetIconRenderParameters(UImage* Icon);

	/** [local] */
	void InitializeWeaponIcons();

public:

	/** [local] */
	void UpdateWeaponIcons(EColorType WeaponColorType);
};
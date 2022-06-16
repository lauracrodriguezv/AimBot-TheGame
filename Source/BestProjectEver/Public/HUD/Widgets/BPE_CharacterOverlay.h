// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BestProjectEver/ColorType.h"
#include "BPE_CharacterOverlay.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
class UBPE_TimerWidget;

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

	//------------------------------------------------------------------------------------------------------------------
	//Health

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	//------------------------------------------------------------------------------------------------------------------
	//Match Time

	/** countdown on match in progress */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBPE_TimerWidget> W_Timer;

	//------------------------------------------------------------------------------------------------------------------
	//Score

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemiesText;

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
	
	void UpdateHealthDisplay(float Health, float MaxHealth);

	/** show the player the time left to end current match state */
	void UpdateMatchTimer(const float TimeLeft);

	void UpdateScoreText(const float Score);

	void UpdateEnemiesAliveText(const int32 EnemiesAlive);
};
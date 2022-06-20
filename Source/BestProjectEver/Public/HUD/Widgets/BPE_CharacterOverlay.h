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
class UBPE_CrosshairWidget;


USTRUCT()
struct FWeaponIcon
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TObjectPtr<UImage> Icon;
	
	FLinearColor IconColor;

	/** render scale when a weapon icon is selected */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D SelectedScale;

	/** render scale when a weapon icon is not selected, by default in the widget blueprint the icons are set to this scale */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FVector2D NotSelectedScale;
	
	FWeaponIcon() : Icon(nullptr), IconColor(FLinearColor::Transparent), SelectedScale(FVector2D(2.0f, 2.0f)),
		NotSelectedScale(FVector2D(0.8f, 0.8f)) {};
	
	FWeaponIcon(UImage* Image, FLinearColor NewColor) : Icon(Image), IconColor(NewColor), SelectedScale(FVector2D(2.0f, 2.0f)),
		NotSelectedScale(FVector2D(0.8f, 0.8f)) {};
};

UCLASS()
class BESTPROJECTEVER_API UBPE_CharacterOverlay : public UUserWidget
{
	
	GENERATED_BODY()

protected:

	//------------------------------------------------------------------------------------------------------------------
	//Weapon
	
	/** left weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* LeftWeaponIcon;

	/** center weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* CenterWeaponIcon;

	/** right weapon icon */
	UPROPERTY(meta = (BindWidget))
	UImage* RightWeaponIcon;

	/** current index of weapon images */
	int32 WeaponIndex;
	
	/** weapon icons set depending on current weapon equipped */
	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	TArray<UImage*> WeaponImages;

	/** current weapon icon selected */
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	UImage* CurrentIconSelected;

	/** crosshair widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBPE_CrosshairWidget> W_Crosshair;

	/** map to access the icons with the color type of the weapon */
	TMap<EColorType, FWeaponIcon> WeaponIcons;

	TMap<EColorType, FLinearColor> CurrentWeaponColor;

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

	/** [local] change icons render properties depending on current weapon color type */
	UFUNCTION()
	void SetIconRenderParameters(EColorType WeaponColorType);

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
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "BestProjectEver/HealthData.h"
#include "GameFramework/HUD.h"
#include "BPE_HUD.generated.h"

class UBPE_CharacterOverlay;
class UUserWidget;
class UBPE_HealthComponent;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_HUD : public AHUD
{
	GENERATED_BODY()

public:

	/** widget with all hud information */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	/** character overlay widget reference */
	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<UBPE_CharacterOverlay> CharacterOverlay;

protected:

	virtual void BeginPlay() override;

	/** update health information on character overlay widget */
	UFUNCTION()
	void UpdateHealth(const FHealthData& HealthData);

public:

	virtual void DrawHUD() override;

	/** Bind character's delegates */
	void BindDelegates();

	/** Create and add character overlay widget to the viewport */
	UFUNCTION()
	void AddCharacterOverlay();

	/** Remove character overlay widget to the viewport */
	UFUNCTION()
	void RemoveCharacterOverlay();
	
	/** update weapon icon depending on the color type of the current weapon */
	UFUNCTION()
	void UpdateCurrentWeaponIcon(const EColorType WeaponColorType);
};

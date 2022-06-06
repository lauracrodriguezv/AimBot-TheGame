// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "GameFramework/HUD.h"
#include "BPE_HUD.generated.h"

class UBPE_CharacterOverlay;
class UUserWidget;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_HUD : public AHUD
{
	GENERATED_BODY()

public:

	/** Widget with all hud information */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	UBPE_CharacterOverlay* CharacterOverlay;

protected:

	virtual void BeginPlay() override;

	void InitiliazeReferences();

public:

	virtual void DrawHUD() override;
	
	UFUNCTION()
	void AddCharacterOverlay();

	UFUNCTION()
	void UpdateCurrentWeapon(EColorType WeaponColorType);
};

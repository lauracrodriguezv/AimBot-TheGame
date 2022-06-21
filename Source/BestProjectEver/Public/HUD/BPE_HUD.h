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
class ABPE_PlayerController;
class ABPE_GameState;
class UBPE_TimerWidget;
class UBPE_AnnouncementOverlay;
class UBPE_ResultsOverlay;
class ABPE_PlayerState;
class UBPE_PauseMenu;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_HUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	FName MatchState;

	/** widget with all hud information */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	
	/** character overlay widget reference */
	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<UBPE_CharacterOverlay> CharacterOverlay;

	/** widget when match is WaitingToStart */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> AnnouncementOverlayClass;

	/** widget when match is WaitingToStart reference */
	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<UBPE_AnnouncementOverlay> AnnouncementOverlay;

	/** widget when match is in Cooldown time */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> ResultsOverlayClass;

	/** widget when match is Cooldown time reference */
	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<UBPE_ResultsOverlay> ResultsOverlay;
	
	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	TObjectPtr<ABPE_GameState> GameStateReference;

	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<ABPE_PlayerState> PlayerStateReference;

	/** widget when match is in Cooldown time */
	UPROPERTY(EditAnywhere, Category = "Player State")
	TSubclassOf<UUserWidget> PauseMenuClass;

	/** widget when match is Cooldown time reference */
	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	TObjectPtr<UBPE_PauseMenu> PauseMenu;

protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	/** update health information on character overlay widget */
	UFUNCTION()
	void UpdateHealth(const FHealthData& HealthData);

	/** update weapon icon depending on the color type of the current weapon */
	UFUNCTION()
	void UpdateCurrentWeaponIcon(const EColorType WeaponColorType);

	/** show the player the time left to end current match state */
	UFUNCTION()
	void UpdateMatchTimeLeft(const float TimeLeft);

	/** update overlay widget on new match state */
	UFUNCTION()
	void UpdateOverlay(const FName NewMatchState);

	/** Create and add Announcement overlay widget to the viewport */
	UFUNCTION()
	void AddAnnouncementOverlay();

	/** Remove Announcement overlay widget to the viewport */
	UFUNCTION()
	void RemoveAnnouncementOverlay();

	/** Create and add results overlay widget to the viewport */
	UFUNCTION()
	void AddResultsOverlay();

	/** update player score on player state score changed */
	UFUNCTION()
	void UpdateScore(const float Score);

	/** update total enemies alive on the match */
	UFUNCTION()
	void UpdateEnemiesAlive(const int32 EnemiesAlive);

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

	UFUNCTION()
	void AddPauseMenu(bool bWasPausedByOtherPlayer);
};

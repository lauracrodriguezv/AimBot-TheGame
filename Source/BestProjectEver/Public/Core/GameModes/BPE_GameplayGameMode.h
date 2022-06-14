// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BPE_GameplayGameMode.generated.h"

class ABPE_BaseCharacter;
class ABPE_Enemy;
class ABPE_PlayerCharacter;
class ABPE_GameState;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_GameplayGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game Mode")
	float TimeLeft;

	UPROPERTY(EditDefaultsOnly, Category="Game Mode")
	float MatchTime;
	
	UPROPERTY(EditDefaultsOnly, Category="Game Mode")
	float WarmupTime;

	float LevelStartingTime;

	/** rate to update time left */
	UPROPERTY(Transient)
	FTimerHandle TimerHandle_RefreshTimeLeftRate;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_GameState> GameStateReference;
	
public:

	ABPE_GameplayGameMode();

protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	virtual void HandleMatchIsWaitingToStart() override;
	
	/** set scores */
	void HandlePlayerDeath(AController* KillerController, AController* KilledController, ABPE_PlayerCharacter* KilledCharacter);

	/** set scores */
	void HandleEnemyDeath(AController* KillerController, AController* KilledController, ABPE_Enemy* KilledCharacter);

	void UpdateTimeLeft();
	
public:
	
	/** notify about kills */
	void OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter);

	/** tries to spawn the player's pawn, at the location returned by FindPlayerStart */
	void RespawnPlayer(AController* KilledController, APawn* KilledCharacter);

	float GetTimeLeft() const { return TimeLeft; };
};

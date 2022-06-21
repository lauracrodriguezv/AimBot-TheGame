// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BPE_GameplayGameMode.generated.h"

class ABPE_BaseCharacter;
class ABPE_Enemy;
class ABPE_PlayerCharacter;
class ABPE_GameState;

namespace MatchState
{
	extern BESTPROJECTEVER_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer
}

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_GameplayGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	//------------------------------------------------------------------------------------------------------------------
	//Match Time
	
	/** time left to end the current match state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game Mode")
	float TimeLeft;

	/** total time of the gameplay match */
	UPROPERTY(EditDefaultsOnly, Category="Game Mode")
	float MatchTime;

	/** total time previous to start match */
	UPROPERTY(EditDefaultsOnly, Category="Game Mode")
	float WarmupTime;

	/** total time previous to start match */
	UPROPERTY(EditDefaultsOnly, Category="Game Mode")
	float CooldownTime;

	/** current time since map starts */
	float LevelStartingTime;

	/** rate to update time left */
	UPROPERTY(Transient)
	FTimerHandle TimerHandle_RefreshTimeLeftRate;

	//------------------------------------------------------------------------------------------------------------------

	UPROPERTY(Transient)
	TObjectPtr<ABPE_GameState> GameStateReference;
	
public:

	ABPE_GameplayGameMode();

protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	/** Called when the state transitions to WaitingToStart */
	virtual void HandleMatchIsWaitingToStart() override;

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted() override;

	/** Called when the state transitions to Cooldown */
	void HandleCooldownTime();

	/** Transition from WaitingToStart to InProgress */
	virtual void StartMatch() override;

	/** Transition from InProgress to Cooldown */
	void StartCooldown();
	
	/** set scores */
	void HandlePlayerDeath(AController* KillerController, AController* KilledController, ABPE_PlayerCharacter* KilledCharacter);

	/** set scores */
	void HandleEnemyDeath(AController* KillerController, AController* KilledController, ABPE_Enemy* KilledCharacter);

	/** Update the time left to finish the current state */
	void UpdateTimeLeft();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
public:
	
	/** notify about kills */
	void OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter);

	/** tries to spawn the player's pawn, at the location returned by FindPlayerStart */
	void RespawnPlayer(AController* KilledController, APawn* KilledCharacter);

	float GetTimeLeft() const { return TimeLeft; };
};

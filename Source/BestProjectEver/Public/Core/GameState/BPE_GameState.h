// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BestProjectEver/MatchDefinitions.h"
#include "BPE_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeLeftUpdated, const float, TimeLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateSet, const FName, MatchState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDies, const int32, EnemiesAlive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetPause, const bool, bPause);

class ABPE_GameplayGameMode;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_GameState : public AGameState
{
	GENERATED_BODY()

protected:

	uint8 bAreAllEnemiesDead : 1;
	
	/** time left to end current match state */
	UPROPERTY(ReplicatedUsing=OnRep_TimeLeft, VisibleAnywhere, BlueprintReadOnly, Category="Game Mode")
	float TimeLeft;

	/** current enemies amount on the match */
	UPROPERTY(ReplicatedUsing=OnRep_EnemiesAlive, BlueprintReadOnly, Category="Game Mode")
	int32 EnemiesAlive;
	
	UPROPERTY(Replicated)
	EMatchResult MatchResult;

	UPROPERTY(Transient)
	TArray<TObjectPtr<APlayerState>> PlayersWhoPaused;

public:

	ABPE_GameState();

	UPROPERTY(BlueprintAssignable)
	FOnTimeLeftUpdated OnTimeLeftUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnMatchStateSet OnMatchStateSet;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDies OnEnemyDeath;

	UPROPERTY(BlueprintAssignable)
	FOnSetPause OnSetPause;
	
protected:

	virtual void BeginPlay() override;

	void SetEnemiesOnMatch();

	/** [client] rep handler on time left updated in server */
	UFUNCTION()
	void OnRep_TimeLeft();

	UFUNCTION()
	void OnRep_EnemiesAlive();

	/** update player pause state on set pause */
	void UpdatePlayerPauseState();
	
public:

	virtual void OnRep_MatchState() override;
	
	void SetTimeLeft(float Time);

	float GetTimeLeft() const { return TimeLeft; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void DecreaseEnemiesAlive();

	int32 GetEnemiesOnMatch() const { return EnemiesAlive; }

	/** Called when the state transitions to Cooldown */
	void DetermineMatchResult();

	EMatchResult GetMatchResult() const { return MatchResult; }

	bool AreAllEnemiesDead() const { return bAreAllEnemiesDead; }
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetPause(bool bPause, APlayerState* InstigatedBy);

	bool IsGamePaused() const { return !PlayersWhoPaused.IsEmpty(); }

	bool WasPauseInstigatedByPlayer(const APlayerState* Player) const;
	
	TArray<APlayerState*> GetTopScoringPlayers() const;
};
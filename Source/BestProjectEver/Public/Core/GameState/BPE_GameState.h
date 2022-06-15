// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BPE_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeLeftUpdated, const float, TimeLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateSet, const FName, MatchState);

class ABPE_GameplayGameMode;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_GameState : public AGameState
{
	GENERATED_BODY()

protected:

	/** time left to end current match state */
	UPROPERTY(ReplicatedUsing=OnRep_TimeLeft, VisibleAnywhere, BlueprintReadOnly, Category="Game Mode")
	float TimeLeft;

	UPROPERTY()
	TObjectPtr<ABPE_GameplayGameMode> GameModeReference;

public:

	ABPE_GameState();

	UPROPERTY(BlueprintAssignable)
	FOnTimeLeftUpdated OnTimeLeftUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnMatchStateSet OnMatchStateSet;
	
protected:

	virtual void BeginPlay() override;

	/** [client] rep handler on time left updated in server */
	UFUNCTION()
	void OnRep_TimeLeft();

public:

	virtual void OnRep_MatchState() override;
	
	void SetTimeLeft(float Time);

	float GetTimeLeft() const { return TimeLeft; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

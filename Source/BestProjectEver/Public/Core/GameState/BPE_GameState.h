// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameState.h"
#include "BPE_GameState.generated.h"

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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Game Mode")
	float TimeLeft;

	UPROPERTY()
	TObjectPtr<ABPE_GameplayGameMode> GameModeReference;

public:

	ABPE_GameState();
	
protected:

	virtual void BeginPlay() override;

public:

	void SetTimeLeft(float Time) { TimeLeft = Time; }

	float GetTimeLeft() const { return TimeLeft; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

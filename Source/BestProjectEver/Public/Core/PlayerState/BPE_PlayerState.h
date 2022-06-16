// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BPE_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, const float, Score);

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ABPE_PlayerState();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnScoreChanged OnScoreChanged;

	/** [client] score rep handler */
	virtual void OnRep_Score() override;

	/** [client and server] score changed handler */
	void OnScoreIncreased();

	UFUNCTION()
	void IncreaseScore(const float ScoreAmount);
};

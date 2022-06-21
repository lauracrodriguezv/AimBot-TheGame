// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPE_PlayerController.generated.h"

class ABPE_HUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameIsPaused, APlayerController*, PlayerWhoPaused);

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** all player inputs are enabled */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Controller")
	uint8 bAreGameplayInputsEnabled : 1;
	
	UPROPERTY(BlueprintReadOnly, Category="UI")
	ABPE_HUD* HUD;

	/** Minimum time before player can respawn after dying. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameMode", meta=(ClampMin=0.0f))
	float TimeToRespawn;

	/** Handle for get HUD once is valid */
	FTimerHandle TimerHandle_HUD;

	/** Handle for efficient management of Respawn timer */
	FTimerHandle TimerHandle_Respawn;

	uint8 bCanUnPause : 1;

	FOnGameIsPaused OnGameIsPaused;

public:

	ABPE_PlayerController();
	
protected:

	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnMatchStateChanged(const FName MatchState);
	
	/** [server] State entered when inactive */
	virtual void BeginInactiveState() override;

	/** [client and server] State entered when match state is in cooldown */
	void BeginCooldownState();

	/** [server] request the game mode to destroy and respawn player's pawn */
	UFUNCTION()
	void RequestRespawn();

	/** [client] get hud and bind functions */
	UFUNCTION(Client, Reliable)
	void Client_UpdateHud();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnGamePaused(APlayerState* PlayerWhoPaused);

public:

	/** check if all player inputs are enabled or just camera movements */
	bool AreGameplayInputsEnabled() const { return  bAreGameplayInputsEnabled; }

	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate) override;
};

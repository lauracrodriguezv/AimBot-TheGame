// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPE_PlayerController.generated.h"

class ABPE_HUD;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category="UI")
	ABPE_HUD* HUD;

	/** Minimum time before player can respawn after dying. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameMode", meta=(ClampMin=0.0f))
	float TimeToRespawn;

	/** Handle for get HUD once is valid */
	FTimerHandle TimerHandle_HUD;

	/** Handle for efficient management of Respawn timer */
	FTimerHandle TimerHandle_Respawn;

public:

	ABPE_PlayerController();
	
protected:

	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	/** [server] State entered when inactive */
	virtual void BeginInactiveState() override;

	/** [server] request the game mode to destroy and respawn player's pawn */
	UFUNCTION()
	void RequestRespawn();

	/** [client] get hud and bind functions */
	UFUNCTION(Client, WithValidation, Reliable)
	void Client_UpdateHud();
};

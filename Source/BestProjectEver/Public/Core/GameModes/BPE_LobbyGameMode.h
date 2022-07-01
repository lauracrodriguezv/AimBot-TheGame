// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BPE_LobbyGameMode.generated.h"

class ABPE_BaseCharacter;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_LobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ABPE_LobbyGameMode();
	
public:

	/** check how many players are to start the travel to the gameplay map */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool TravelToMap(const FString& InURL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);

	/** notify about kills */
	void OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter);

	/** tries to spawn the player's pawn, at the location returned by FindPlayerStart */
	void RespawnPlayer(AController* KilledController, APawn* KilledCharacter);
};

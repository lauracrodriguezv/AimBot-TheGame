// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BPE_GameplayGameMode.generated.h"

class ABPE_BaseCharacter;
class ABPE_Enemy;
class ABPE_PlayerCharacter;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_GameplayGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ABPE_GameplayGameMode();

protected:

	/** set scores */
	void HandlePlayerDeath(AController* KillerController, AController* KilledController, ABPE_PlayerCharacter* KilledCharacter);

	/** set scores */
	void HandleEnemyDeath(AController* KillerController, AController* KilledController, ABPE_Enemy* KilledCharacter);
	
public:

	/** notify about kills */
	void OnCharacterDeath(AController* KillerController, AController* KilledController, ABPE_BaseCharacter* KilledCharacter);

	/** tries to spawn the player's pawn, at the location returned by FindPlayerStart */
	void RespawnPlayer(AController* KilledController, APawn* KilledCharacter);
};

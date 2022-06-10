// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BPE_BaseCharacter.h"
#include "BestProjectEver/EnemyDefinitions.h"
#include "BPE_Enemy.generated.h"

class ABPE_PathFollowing;
class ABPE_Weapon;

UCLASS()
class BESTPROJECTEVER_API ABPE_Enemy : public ABPE_BaseCharacter
{
	GENERATED_BODY()

public:

	ABPE_Enemy();
	
protected:

	//------------------------------------------------------------------------------------------------------------------
	//Enemy State
	
	EEnemyStatus EnemyStatus;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy State")
	TMap<EEnemyStatus, float> EnemySpeedMap;

	/** target AI controller is perceived */
	FVector TargetViewLocation;

	/** patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrol")
	TObjectPtr<ABPE_PathFollowing> PatrolPathReference;

	//------------------------------------------------------------------------------------------------------------------
	//Weapon

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ABPE_Weapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<ABPE_Weapon> CurrentWeapon;

protected:

	virtual void BeginPlay() override;

	/** spawn enemy's weapon by default */
	void SpawnWeapon();

	/** set current enemy status handler */
	void OnSetEnemyStatus(EEnemyStatus NewEnemyStatus);
	
public:

	//------------------------------------------------------------------------------------------------------------------
	// Weapon
	
	/** weapon perform fire. The enemy's weapon is not automatic by default */
	virtual void StartWeaponFire() override;
	
	virtual void StopWeaponFire() override;

	virtual bool IsWeaponEquipped() const override;

	//------------------------------------------------------------------------------------------------------------------
	// Enemy data
	
	/** returns patrol path assigned */
	ABPE_PathFollowing* GetPathFollowing() const { return PatrolPathReference; }

	/** set current enemy status */
	EEnemyStatus GetEnemyStatus() const { return EnemyStatus; }

	/** set current enemy status */
	void SetEnemyStatus(EEnemyStatus NewEnemyStatus);

	/** so enemy knows which direction to shoot */
	virtual FRotator GetViewRotation() const override;

	/** set by the AI controller if an actor is perceived */
	void SetTargetViewLocation(FVector TargetLocation) { TargetViewLocation = TargetLocation; };
};

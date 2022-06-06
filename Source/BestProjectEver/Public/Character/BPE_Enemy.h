// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BPE_BaseCharacter.h"
#include "BPE_Enemy.generated.h"

class ABPE_PathFollowing;
class ABPE_Weapon;

UENUM(BlueprintType)
enum class EEnemyStatus : uint8 
{
	Patrol,
	Investigating,
	Combat
};

UCLASS()
class BESTPROJECTEVER_API ABPE_Enemy : public ABPE_BaseCharacter
{
	GENERATED_BODY()

public:

	ABPE_Enemy();
	
protected:

	//------------------------------------------------------------------------------------------------------------------
	//Enemy State
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrol")
	TObjectPtr<ABPE_PathFollowing> PatrolPathReference;
	
	EEnemyStatus EnemyStatus;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy State")
	TMap<EEnemyStatus, float> EnemySpeedMap;

	//------------------------------------------------------------------------------------------------------------------
	//Weapon

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ABPE_Weapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<ABPE_Weapon> CurrentWeapon;

protected:

	virtual void BeginPlay() override;

	void SpawnWeapon();
	
	virtual void StartWeaponFire() override;

	virtual void StopWeaponFire() override;
	
public:

	UFUNCTION(BlueprintCallable)
	ABPE_PathFollowing* GetPathFollowing() const { return PatrolPathReference; }

	UFUNCTION(BlueprintCallable)
	void SetEnemyStatus(EEnemyStatus NewEnemyStatus);
};

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
	
	EEnemyStatus EnemyStatus;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy State")
	TMap<EEnemyStatus, float> EnemySpeedMap;

	//------------------------------------------------------------------------------------------------------------------
	//Weapon

	FVector TargetViewLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrol")
	TObjectPtr<ABPE_PathFollowing> PatrolPathReference;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ABPE_Weapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<ABPE_Weapon> CurrentWeapon;

protected:

	virtual void BeginPlay() override;

	void SpawnWeapon();

	void OnSetEnemyStatus(EEnemyStatus NewEnemyStatus);
	
public:

	UFUNCTION(BlueprintCallable)
	virtual void StartWeaponFire() override;

	UFUNCTION(BlueprintCallable)
	virtual void StopWeaponFire() override;
	
	UFUNCTION(BlueprintCallable)
	ABPE_PathFollowing* GetPathFollowing() const { return PatrolPathReference; }

	EEnemyStatus GetEnemyStatus() const { return EnemyStatus; }
	
	UFUNCTION(BlueprintCallable)
	void SetEnemyStatus(EEnemyStatus NewEnemyStatus);

	virtual bool IsWeaponEquipped() const override;

	virtual FRotator GetViewRotation() const override;

	void SetTargetViewLocation(FVector TargetLocation) { TargetViewLocation = TargetLocation; };
};

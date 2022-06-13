// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "Character/BPE_BaseCharacter.h"
#include "BestProjectEver/EnemyDefinitions.h"
#include "Interfaces/BPE_InteractWithColorType.h"
#include "BPE_Enemy.generated.h"

class ABPE_PathFollowing;
class ABPE_Weapon;

UCLASS()
class BESTPROJECTEVER_API ABPE_Enemy : public ABPE_BaseCharacter, public IBPE_InteractWithColorType
{
	GENERATED_BODY()

public:

	ABPE_Enemy();

	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:

	//------------------------------------------------------------------------------------------------------------------
	//Enemy Type
	
	/** to determine which color type weapon can hurt this enemy */
	UPROPERTY(ReplicatedUsing=OnRep_ColorType, EditAnywhere, Category = "Enemy State")
	EColorType ColorType;

	/** Material based on color type */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TMap<EColorType, FLinearColor> MaterialColor;

	/** impulse added when spawn pad drops this actor (OnStopInteraction) */
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float ImpulseOnStopInteraction;

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

	UFUNCTION()
	void OnRep_ColorType();
	
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
	void SetTargetViewLocation(const FVector& TargetLocation) { TargetViewLocation = TargetLocation; }

	//------------------------------------------------------------------------------------------------------------------
	//Color Type

	/** change mesh color depending on color type */
	virtual void UpdateMeshColor() override;
	
	/** get enemy color type */
	virtual EColorType GetColorType() const override { return ColorType; }

	/** set enemy color type */
	virtual void SetColorType(const EColorType NewColorType) override;

	/** game element dropped current actor */
	virtual void OnStopInteraction() override;

	//------------------------------------------------------------------------------------------------------------------
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateMeshPhysics();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "Character/BPE_BaseCharacter.h"
#include "BestProjectEver/EnemyDefinitions.h"
#include "Interfaces/BPE_FollowSplinePath.h"
#include "Interfaces/BPE_InteractWithColorType.h"
#include "BPE_Enemy.generated.h"

USTRUCT(BlueprintType)
struct FEnemyParameterOnNewState
{
	GENERATED_BODY()

	/** Emissive color */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	FLinearColor EmissiveColor;

	/** Color visor */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	FLinearColor ColorVisor;

	/** Emissive animation speed for the enemy's eyes */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	float AnimationEmissiveMultiplier;

	/** Emissive multiplier for the enemy's body */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	float EmissiveIntensity;

	/** Max walk speed */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	float MaxWalkSpeed;
};

class ABPE_PathFollowing;
class ABPE_Weapon;
class UBPE_FollowSplineComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_Enemy : public ABPE_BaseCharacter, public IBPE_InteractWithColorType, public IBPE_FollowSplinePath
{
	GENERATED_BODY()

public:

	ABPE_Enemy();

	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBPE_FollowSplineComponent> FollowSplineComponent;

	//------------------------------------------------------------------------------------------------------------------
	//Enemy Type
	
	/** to determine which color type weapon can hurt this enemy */
	UPROPERTY(ReplicatedUsing=OnRep_ColorType, EditAnywhere, Category = "Enemy State")
	EColorType ColorType;

	/** Material based on color type */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy State")
	TMap<EColorType, FLinearColor> BodyMaterialColor;
	
	/** impulse added when spawn pad drops this actor (OnStopInteraction) */
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float ImpulseOnStopInteraction;

	//------------------------------------------------------------------------------------------------------------------
	//Enemy State

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_EnemyStatus, Category = "Enemy State")
	EEnemyStatus EnemyStatus;

	/** Material based on color type */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy State")
	TMap<EEnemyStatus, FEnemyParameterOnNewState> ParameterOnNewStatus;

	/** target AI controller is perceived */
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	FVector TargetViewLocation;

	/** patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrol")
	TObjectPtr<ABPE_PathFollowing> PatrolPathReference;

	//------------------------------------------------------------------------------------------------------------------
	//Weapon

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ABPE_Weapon> WeaponClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<ABPE_Weapon> CurrentWeapon;

protected:

	virtual void BeginPlay() override;

	/** spawn enemy's weapon by default */
	void SpawnWeapon();

	UFUNCTION()
	void OnRep_EnemyStatus();
	
	/** set current enemy status handler */
	void OnSetEnemyStatus();

	void UpdateMaterialOnEnemyStatus();
	
	virtual void HandleCharacterDeath(AActor* DamagedActor, AController* InstigatedBy, AActor* DamageCauser) override;
	
	virtual void DropWeapon() override;
	
	UFUNCTION()
	void OnRep_ColorType();
	
	virtual void FollowSplinePath_Implementation(const FVector& NextPointLocation) override;

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

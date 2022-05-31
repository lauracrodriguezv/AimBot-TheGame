// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BPE_BaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "BPE_Weapon.generated.h"

class USphereComponent;
class UWidgetComponent;
class ABPE_Projectile;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading	UMETA(DisplayName = "Reloading"),
	Equipped	UMETA(DisplayName = "Equipped")
};

UENUM(BlueprintType)
enum class EWeaponColorType : uint8
{
	Yellow		UMETA(DisplayName = "Yellow"),
	Blue		UMETA(DisplayName = "Blue"),
	Red			UMETA(DisplayName = "Red")
};

UCLASS()
class BESTPROJECTEVER_API ABPE_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABPE_Weapon();

protected:

	//------------------------------------------------------------------------------------------------------------------
	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/** Only the server can detect overlap events with this area*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<USphereComponent> PickupArea;

	/** press E for pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="User Interface")
	TObjectPtr<UWidgetComponent> PickupWidget;

	//------------------------------------------------------------------------------------------------------------------

	TObjectPtr<ABPE_BaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<ABPE_Projectile> ProjectileBullet;
	
	/** current weapon state */
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState)
	EWeaponState CurrentState;

	/**  */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	EWeaponColorType ColorType;

	//------------------------------------------------------------------------------------------------------------------
	//Weapon Data
	
	bool bIsAutomatic;

	bool bCanFire;

	float RoundsPerMinute;
	
	/** Handle for efficient management of HandleFiring timer */
	FTimerHandle TimerHandle_AutoFire;

	/** time between two consecutive shots */
	float TimeBetweenShots;

	/** time of last successful weapon fire */
	float LastFireTime;

	float ShotDistance;
	
	FHitResult HitTarget;

protected:
	
	virtual void BeginPlay() override;
	
	/** To set overlapping weapon on the character */
	UFUNCTION()
	virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

	void OnSetWeaponState();

	void SetWeaponParametersOnNewState(ECollisionEnabled::Type MeshTypeCollision, bool bEnableMeshPhysics,
		ECollisionEnabled::Type PickupAreaTypeCollision);

	UFUNCTION()
	virtual void Fire();

	void TraceUnderCrosshairs(FHitResult& OutTraceHitResult);

public:
	
	//------------------------------------------------------------------------------------------------------------------
	// Reading data

	virtual void SetOwner(AActor* NewOwner) override;
	
	/** get current weapon state */
	EWeaponState GetCurrentState() const { return CurrentState; }

	/** set weapon state */
	void SetState(EWeaponState State);

	/* Called from the server and also from the client with a replicate variable*/
	void SetWidgetVisibility(bool bShowWidget);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartFire();

	void StopFire();

	void EnableFire();
};

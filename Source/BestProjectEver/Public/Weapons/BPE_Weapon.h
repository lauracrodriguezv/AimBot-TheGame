// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BPE_PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "BPE_Weapon.generated.h"

class USphereComponent;
class UWidgetComponent;
class ABPE_Projectile;
class UAnimationAsset;
class ABPE_BaseCharacter;
class ABPE_Casing;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading,
	Equipped
};

UENUM(BlueprintType)
enum class EWeaponColorType : uint8
{
	Yellow,
	Blue,
	Red
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
	//Weapon Data

	TObjectPtr<ABPE_BaseCharacter> OwnerCharacter;
	
	/** bullet class that the weapon spawn */
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<ABPE_Projectile> BulletClass;
	
	/** current weapon state */
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState)
	EWeaponState CurrentState;

	/** to determine which enemy it can apply damage */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon State")
	EWeaponColorType ColorType;

	/** socket where bullet is spawned */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon State")
	FName MuzzleFlashSocketName;

	/** socket where casing is spawned */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon State")
	FName AmmoEjectSocketName;
	
	uint8 bIsAutomatic : 1;

	/** true by default and when TimerHandle_AutoFire has finished */
	uint8 bCanFire : 1;

	/** the ability of the weapon to fire specific number of shots per unit of time */
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0.1), Category= "Weapon State")
	float RoundsPerMinute;
	
	/** time between two consecutive shots */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Weapon State")
	float TimeBetweenShots;

	/** distance from the weapon socket to the end location of the trace if there was no blocking hit */
	float ShotDistance;

	/** speed to interpolate from the default field of view of the player to the ZoomedFOV when is aiming */
	UPROPERTY(EditAnywhere, Category = "Aiming")
	float ZoomInterpSpeed;

	/** FOV when is aiming */
	UPROPERTY(EditAnywhere, Category = "Aiming")
	float ZoomedFOV;

	/** Handle for efficient management of Firing timer */
	FTimerHandle TimerHandle_AutoFire;

	/** impact point location of the fire line trace */
	FVector HitTarget;

	/** weapon animation */
	UPROPERTY(EditAnywhere, Category = " Effects")
	TObjectPtr<UAnimationAsset> FireAnimation;

	/** spawned actor for ammo FX */
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<ABPE_Casing> CasingClass;

protected:
	
	virtual void BeginPlay() override;

	void InitializeReferences();
	
	/** [server] To set overlapping weapon on the character */
	UFUNCTION()
	virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** [server] To set overlapping weapon on the character to nullptr */
	UFUNCTION()
	virtual void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	//------------------------------------------------------------------------------------------------------------------
	//Weapon state
	
	/** [client] called when CurrentState is set and it calls OnSetWeaponState */
	UFUNCTION()
	void OnRep_WeaponState();

	/* [client and server] Set weapon parameters depending on the current new state */
	void OnSetWeaponState();

	/* [client and server] enable or disable collisions and pickup widget visibility depending on the weapon state */
	void SetWeaponParametersOnNewState(ECollisionEnabled::Type MeshTypeCollision, bool bEnableMeshPhysics,
		ECollisionEnabled::Type PickupAreaTypeCollision);

	//------------------------------------------------------------------------------------------------------------------
	//Weapon usage helpers
	
	/* [server] fire action, can be delayed to satisfy TimeBetweenShots */ 
	UFUNCTION()
	virtual void Fire();

	/** [server] perform trace to set hit target */
	void TraceUnderCrosshairs();

	/** [server] prevent to spawn multiple times fire button and fire again if it is on firing state and is automatic */
	void HandleReFiring();

	//------------------------------------------------------------------------------------------------------------------
	//Effects 
	
	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void Multicast_PlayFireEffects(const FVector ImpactPoint);

public:
	
	virtual void Tick(float DeltaSeconds) override;
	
	//------------------------------------------------------------------------------------------------------------------
	// Reading data

	virtual void SetOwner(AActor* NewOwner) override;
	
	/** get current weapon state */
	EWeaponState GetCurrentState() const { return CurrentState; }

	/** [server] set weapon state */
	void SetState(EWeaponState State);

	/** [client and server] called when character start or end overlapping pickup area if it is locally controlled*/
	void SetWidgetVisibility(bool bShowWidget);

	float GetZoomedFOV() const { return ZoomedFOV; }

	float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	//------------------------------------------------------------------------------------------------------------------
	//Weapon usage 
	
	/* [server] start firing, can be delayed to satisfy TimeBetweenShots */ 
	void StartFire();

	/* [server] start firing, can be delayed to satisfy TimeBetweenShots */ 
	void StopFire();
	
	//------------------------------------------------------------------------------------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


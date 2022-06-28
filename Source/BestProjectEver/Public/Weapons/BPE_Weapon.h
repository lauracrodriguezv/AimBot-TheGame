// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BestProjectEver/ColorType.h"
#include "Interfaces/BPE_InteractWithColorType.h"
#include "BPE_Weapon.generated.h"

class USphereComponent;
class UWidgetComponent;
class ABPE_Projectile;
class UAnimationAsset;
class ABPE_BaseCharacter;
class ABPE_Casing;
class USoundCue;
class UMaterialInstanceConstant;
class USoundBase;
class ABPE_PlayerCharacter;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading,
	Equipped,
	WaitingToDestroy
};

UENUM(BlueprintType)
enum class EShootType : uint8
{
	LineTrace,
	Bullet,
	Mixed
};

UCLASS()
class BESTPROJECTEVER_API ABPE_Weapon : public AActor, public IBPE_InteractWithColorType
{
	GENERATED_BODY()
	
public:	
	
	ABPE_Weapon();

	virtual void OnConstruction(const FTransform& Transform) override;

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
	//Weapon Type

	/** to determine which enemy it can apply damage */
	UPROPERTY(ReplicatedUsing=OnRep_ColorType, BlueprintReadOnly, Category= "Weapon State")
	EColorType CurrentColorType;

	/** Color set at the beginning of the match */
	UPROPERTY(ReplicatedUsing=OnRep_ColorType, EditAnywhere, Category = "Weapon State")
	EColorType DefaultColorType; 

	/** Material based on color type */
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TMap<EColorType, FLinearColor> MaterialColor;
	
	//------------------------------------------------------------------------------------------------------------------
	//Weapon Data
	
	TObjectPtr<ABPE_BaseCharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_PlayerCharacter> PlayerOwner;
	
	/** current weapon state */
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState)
	EWeaponState CurrentState;

	/** socket where bullet is spawned */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon State")
	FName MuzzleFlashSocketName;

	/** socket where casing is spawned */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon State")
	FName AmmoEjectSocketName;

	UPROPERTY(EditAnywhere, Category= "Weapon State")
	uint8 bIsAutomatic : 1;

	/** true by default and when TimerHandle_AutoFire has finished */
	uint8 bCanFire : 1;

	/** Material fired against the selected target */
	UPROPERTY(ReplicatedUsing=OnRep_CurrentAmmo, BlueprintReadOnly, Category="Ammo")
	int32 CurrentAmmo;

	/** Max ammunition weapon can hold */
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 MagCapacity;

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.0), Category= "Weapon State")
	float BaseDamage;
	
	/** the ability of the weapon to fire specific number of shots per unit of time */
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0.1), Category= "Weapon State")
	float RoundsPerMinute;
	
	/** time between two consecutive shots */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Weapon State")
	float TimeBetweenShots;

	/** distance from the weapon socket to the end location of the trace if there was no blocking hit */
	float ShotDistance;

	/** additional distance for shooting trace start to prevent the shoot hit something behind the character  */
	UPROPERTY(EditDefaultsOnly, Category="Weapon State")
	float ExtraDistance;

	/** speed to interpolate from the default field of view of the player to the ZoomedFOV when is aiming */
	UPROPERTY(EditAnywhere, Category = "Aiming")
	float ZoomInterpSpeed;

	/** FOV when is aiming */
	UPROPERTY(EditAnywhere, Category = "Aiming")
	float ZoomedFOV;

	/** Magnitude for impulse added when weapon is dropped */
	UPROPERTY(EditDefaultsOnly, Category="Weapon State")
	float ImpulseOnDropped;

	/** Minimum time before actor is destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon State", meta=(ClampMin=0.0f))
	float DestroyDelay;

	UPROPERTY(EditDefaultsOnly, Category= "Weapon State")
	TSubclassOf<UDamageType> DamageType;

	/** Handle for efficient management of Firing timer */
	FTimerHandle TimerHandle_AutoFire;

	//------------------------------------------------------------------------------------------------------------------
	//Animation
	
	/** weapon animation */
	UPROPERTY(EditAnywhere, Category = " Effects")
	TObjectPtr<UAnimationAsset> FireAnimation;

	//------------------------------------------------------------------------------------------------------------------
	//Effects and Sounds
	
	/** spawned actor for ammo FX */
	UPROPERTY(EditAnywhere, Category = "Effects")
	
	TSubclassOf<ABPE_Casing> CasingClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ShootLoudness;

	//------------------------------------------------------------------------------------------------------------------
	//Shoot Type
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon State")
	EShootType ShootType;

	UPROPERTY(EditAnywhere, Category = "Weapon State")
	TSubclassOf<ABPE_Projectile> BulletClass;

	/** min distance to spawn a bullet to be replicated */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "ShootType == EShootType::Mixed"), Category = "Weapon State")
	float MinDistanceToImpactPoint;

protected:
	
	virtual void BeginPlay() override;

	void InitializeReferences();
	
	/** [server] called when something starts overlaps pickup area component */
	UFUNCTION()
	virtual void OnPickupAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** [server] called when something stops overlaps pickup area component */
	UFUNCTION()
	virtual void OnPickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	//------------------------------------------------------------------------------------------------------------------
	//Shoot Type

	/* [server] spawn effects in impact point with a multicast */ 
	void ShootWithLineTrace(const FVector& ImpactPoint);

	/* [server] spawn bullets, the initial projectile speed might be slow because of replication */ 
	void ShootWithBullets(const FVector& ImpactPoint);

	/* [server] spawn a bullet depending on MinDistanceToImpactPoint */ 
	void ShootWithLineTraceAndBullet(const FVector& TraceStart, const FVector& ImpactPoint);

	//------------------------------------------------------------------------------------------------------------------
	//Weapon state

	/** [server] set weapon state */
	void SetState(EWeaponState State);
	
	void HandleIdleState();
	
	void HandleEquippedState();

	void HandleWaitingToDestroyState();

	/** set physics properties so the weapon can be equipped */
	void SetPhysicsPropertiesToEquip();

	/** [client] replicate weapon color material */
	UFUNCTION()
	void OnRep_ColorType();
	
	/** [client] called when CurrentState is set and it calls OnSetWeaponState */
	UFUNCTION()
	void OnRep_WeaponState();

	/* [client and server] Set weapon parameters depending on the current new state */
	void OnSetWeaponState();

	/* [client and server] enable or disable collisions depending on the weapon state */
	void UpdatePhysicsProperties(ECollisionEnabled::Type MeshTypeCollision, bool bEnableMeshPhysics,
		ECollisionEnabled::Type PickupAreaTypeCollision);
	
	//------------------------------------------------------------------------------------------------------------------
	//Weapon usage helpers
	
	/* [server] fire action, can be delayed to satisfy TimeBetweenShots */ 
	UFUNCTION()
	virtual void Fire();

	/** check if enemy hit has the same color type */
	void ApplyDamageOnHit(const FHitResult& HitResult);
	
	void ApplyDamage(const FHitResult& HitResult);

	/** [server] perform trace to set hit target */
	void TraceUnderCrosshairs(FHitResult& OutHitResult);

	/** [server] prevent to spawn multiple times fire button and fire again if it is on firing state and is automatic */
	void HandleReFiring();

	/** [client] update owner rep handler */
	virtual void OnRep_Owner() override;

	/** [server and client] on owner is set */ 
	void OnSetNewOwner();
	
	//------------------------------------------------------------------------------------------------------------------
	//Effects 
	
	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void Multicast_PlayMuzzleFireEffects(const FVector& MuzzleLocation);

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void Multicast_PlayImpactFireEffects(const FVector& ImpactPoint);

	//------------------------------------------------------------------------------------------------------------------
	//Ammo

	/** [server] update current weapon ammo
	 * @param Ammo ammunition reloaded or removed to current weapon
	 */
	void UpdateAmmo(const int32 Ammo = -1);
	
	UFUNCTION()
	void OnRep_CurrentAmmo();

	/** [server and client] current ammo has changed */
	void OnAmmoChanged();
	
public:
	
	virtual void Tick(float DeltaSeconds) override;
	
	//------------------------------------------------------------------------------------------------------------------
	// Reading data

	virtual void SetOwner(AActor* NewOwner) override;

	void OnPickup(AActor* NewOwner);

	/** [server] set weapon parameter when is dropped */
	void OnDropped();

	/** [server] update current weapon ammo
	 * @param AmmoReloaded ammunition reloaded or removed to current weapon
	 */
	void OnReloading(const int32 AmmoReloaded);
	
	/** get current weapon state */
	EWeaponState GetCurrentState() const { return CurrentState; }

	/** [client and server] called when character start or end overlapping pickup area if it is locally controlled*/
	void SetWidgetVisibility(bool bShowWidget);

	float GetZoomedFOV() const { return ZoomedFOV; }

	float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	
	FTransform GetMuzzleSocketTransform() const;

	int32 GetMagCapacity() const { return MagCapacity; }

	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	/** Weapon does not have ammo */
	bool IsEmpty() const;

	/** Weapon has all ammunition it can hold */
	bool IsMagFull() const;
	
	//------------------------------------------------------------------------------------------------------------------
	//Weapon usage 
	
	/* [server] start firing, can be delayed to satisfy TimeBetweenShots */ 
	void StartFire();

	/* [server] start firing, can be delayed to satisfy TimeBetweenShots */ 
	void StopFire();

	//------------------------------------------------------------------------------------------------------------------
	//Color Type
	
	/** change mesh color depending on color type */
	virtual void UpdateMeshColorType(const EColorType ColorType) override;

	/** get weapon color type */
	virtual EColorType GetCurrentColorType() const override { return CurrentColorType; }

	/** get weapon default color type */
	virtual EColorType GetDefaultColorType() const override { return DefaultColorType; }

	/** set weapon color type */
	virtual void SetCurrentColorType(const EColorType ColorType) override;

	virtual void SetDefaultColorType(const EColorType ColorType) override;

	/** game element dropped current actor */
	virtual void OnStopInteraction() override;
	
	//------------------------------------------------------------------------------------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


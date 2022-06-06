// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPE_BaseCharacter.h"
#include "BestProjectEver/ColorType.h"
#include "BPE_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ABPE_Weapon;
class UBPE_InventoryComponent;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCurrentWeapon, EColorType, WeaponColorType);

UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerCharacter : public ABPE_BaseCharacter
{
	GENERATED_BODY()

protected:
	
	/** Third person camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

public:

	ABPE_PlayerCharacter();

	/** delegate called when current weapon is set */
	UPROPERTY(BlueprintAssignable)
	FOnChangeCurrentWeapon OnChangeCurrentWeapon;

protected:
	
	/** Camera can be inverted or normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	uint8 bIsLookInverted : 1;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Aiming")
	uint8 bIsAiming : 1;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	float BaseLookUpRate;

	/** Walk speed reference to decrease when is aiming */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float DefaultWalkSpeed;

	/** The maximum ground speed when is aiming */
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float AimWalkSpeed;

	/** the default horizontal field of view of the camera component */
	float DefaultFOV;
	
	/** current field of view when is interpolating for aiming */
	float CurrentFOV;

	/** speed to return to DefaultFOV from CurrentFOV */
	float ZoomOutInterpSpeed;
	
	//------------------------------------------------------------------------------------------------------------------
	// Inventory

	/** set when character is over the pickup area of the weapon */
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	TObjectPtr<ABPE_Weapon> OverlappingWeapon;

	/** Set if overlapping weapon is valid or there are weapons in the inventory and client press EquipWeapon */
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon)
	TObjectPtr<ABPE_Weapon> CurrentWeapon;

	/** weapons in inventory */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Inventory)
	TArray<TObjectPtr<ABPE_Weapon>> Inventory;

	//------------------------------------------------------------------------------------------------------------------
	// Animations

	/** anim instance reference */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimInstance> AnimInstance;

	/** animation played on current weapon change */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SwapWeaponMontage;
	
	//------------------------------------------------------------------------------------------------------------------
	// Sounds And Effects
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> PickupSound;

protected:

	virtual void BeginPlay() override;

	void InitializeReference();
	
	//------------------------------------------------------------------------------------------------------------------
	// Input handlers
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	/* Frame rate independent turn */
	void TurnAtRate(float Value);

	/* Frame rate independent lookup */
	void LookUpAtRate(float Value);

	virtual void AddControllerPitchInput(float Value) override;

	void StartCrouch();

	void EndCrouch();

	/** [client] call the server to perform fire */
	void StartWeaponFire();

	/** [client] call the server to perform stop fire */
	void StopWeaponFire();

	/** [client] call the server to perform aiming state */
	void Aim();

	/** [client] equip overlapping weapon on the server */
	void EquipWeapon();

	/** [client] equip next weapon on the inventory */
	void EquipNextWeapon();

	/** [client] equip previous weapon on the inventory */
	void EquipPreviousWeapon();
	
	//------------------------------------------------------------------------------------------------------------------
	// Weapon

	/** [server] equip weapon */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(ABPE_Weapon* WeaponToEquip);
	
	/** [server] equip overlapping weapon */
	void HandleEquipWeapon(ABPE_Weapon* WeaponToEquip);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bIsPlayerAiming);

	/** [server] set aim walk speed */
	void OnIsAimingChanged();
	
	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_OverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon();

	/** [server and client] called when current weapon change */
	void OnCurrentWeaponChanged();

	/** [client and server] overlapping weapon handler */
	void OnSetOverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartFire();

	/** [server] call weapon's stop fire function */
	void OnStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopFire();

	/** [server] call weapon's fire function */
	void OnStopFire();

	/** [local] set the camera field of view depending on bIsAiming */
	UFUNCTION()
	void InterpolateFieldOfView(float DeltaSeconds);

	//------------------------------------------------------------------------------------------------------------------
	//Inventory

	/** find in inventory the weapon with a specific color */
	ABPE_Weapon* FindWeaponWithColorType(EColorType ColorType) const;

	/** [server] add weapon to inventory */
	void PickupWeapon(ABPE_Weapon* NewWeapon);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAsCurrentWeapon(ABPE_Weapon* Weapon);

	/** [server] hide last weapon used */
	void HideUnusedWeapon(ABPE_Weapon* Weapon);

	/** [client] weapon inventory rep handler */
	UFUNCTION()
	void OnRep_Inventory();

	/** [server and client] weapon inventory handler */
	void OnInventoryChanged();

	//------------------------------------------------------------------------------------------------------------------
	// Sounds And Effects

	/** play sound cue */
	void PlaySound(USoundCue* Sound);

	/** play anim montage */
	void PlayAnimMontage(UAnimMontage* Montage, float PlayRate = 1.0f);

public:

	virtual void Tick(float DeltaSeconds) override;
	
	/** [server] overlapping weapon handler */
	void SetOverlappingWeapon(ABPE_Weapon* Weapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** check if current weapon is valid */
	virtual bool IsWeaponEquipped() const override;

	bool IsAiming() const { return bIsAiming; };

	virtual FVector GetPawnViewLocation() const override;
};


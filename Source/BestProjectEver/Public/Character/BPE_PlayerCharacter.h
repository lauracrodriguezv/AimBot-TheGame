// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPE_BaseCharacter.h"
#include "BPE_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ABPE_Weapon;
/**
 * 
 */
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

protected:
	
	/** Camera can be inverted or normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	uint8 bIsLookInverted : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
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

protected:

	virtual void BeginPlay() override;
	
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
	
	//------------------------------------------------------------------------------------------------------------------
	// Weapon

	/** [client] equip overlapping weapon on the server */
	void EquipWeapon();

	/** [server] equip weapon */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(ABPE_Weapon* WeaponToEquip);
	
	/** [server] equip overlapping weapon */
	void SetEquippedWeapon(ABPE_Weapon* WeaponToEquip, ABPE_Weapon* LastWeapon);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bIsPlayerAiming);

	/** [server] set aim walk speed */
	void SetAiming();
	
	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_OverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon();

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

public:

	virtual void Tick(float DeltaSeconds) override;
	
	/** [server] overlapping weapon handler */
	void SetOverlappingWeapon(ABPE_Weapon* Weapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** for the anim instance */
	virtual bool IsEquipped() const override;

	virtual FVector GetPawnViewLocation() const override;
};


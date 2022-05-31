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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

public:

	ABPE_PlayerCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	bool bIsLookInversion;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	float BaseLookUpRate;

	
	//------------------------------------------------------------------------------------------------------------------
	// Weapon

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	TObjectPtr<ABPE_Weapon> OverlappingWeapon;

	/** Set if overlapping weapon is valid and client press EquipWeapon */
	UPROPERTY(Replicated)
	TObjectPtr<ABPE_Weapon> EquippedWeapon;

protected:

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
	
	//------------------------------------------------------------------------------------------------------------------
	// Weapon

	/** [client] equip overlapping weapon*/
	void EquipWeapon();

	/** equip weapon */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(ABPE_Weapon* WeaponToEquip);
	
	/** [server] equip overlapping weapon */
	void SetEquippedWeapon(ABPE_Weapon* WeaponToEquip, ABPE_Weapon* LastWeapon);
	
	//------------------------------------------------------------------------------------------------------------------

	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_OverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	/** [client and server] overlapping weapon handler */
	void OnSetOverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

public:
	
	/** [server] overlapping weapon handler */
	void SetOverlappingWeapon(ABPE_Weapon* Weapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

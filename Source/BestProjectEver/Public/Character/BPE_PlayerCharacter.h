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
class ABPE_SpawnPad;
class ABPE_PlayerController;
class ABPE_GameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCurrentWeapon, EColorType, WeaponColorType);

UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerCharacter : public ABPE_BaseCharacter
{
	GENERATED_BODY()

protected:

	//------------------------------------------------------------------------------------------------------------------
	// Components
	
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
	FOnChangeCurrentWeapon OnChangeCurrentWeaponDelegate;

protected:
	
	/** Camera can be inverted or normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	uint8 bIsLookInverted : 1;
	
	UPROPERTY(ReplicatedUsing=OnRep_Aiming, BlueprintReadOnly, Category = "Aiming")
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

	UPROPERTY(Transient)
	TObjectPtr<ABPE_GameState> GameStateReference;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_PlayerController> PlayerControllerReference;
	
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

	/** set when character is over the activation trigger of a spawn pad */
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingSpawnPad)
	TObjectPtr<ABPE_SpawnPad> OverlappingSpawnPad;

	//------------------------------------------------------------------------------------------------------------------
	// Animations

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
	// Player State

	/** [server]  */
	virtual void HandleCharacterDeath(AActor* KilledActor, AController* InstigatedBy, AActor* Killer) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;
	
	//------------------------------------------------------------------------------------------------------------------
	// Input handlers
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	/* Frame rate independent turn */
	void TurnAtRate(float Value);

	/* Frame rate independent lookup */
	void LookUpAtRate(float Value);

	virtual void AddControllerPitchInput(float Value) override;

	virtual void AddControllerYawInput(float Val) override;

	virtual void Jump() override;

	virtual void StopJumping() override;
	
	void StartCrouch();

	void EndCrouch();

	/** [client] call the server to perform aiming state */
	void Aim();

	/** [client] equip overlapping weapon on the server */
	void EquipWeapon();

	/** [client] equip next weapon on the inventory */
	void EquipNextWeapon();

	/** [client] equip previous weapon on the inventory */
	void EquipPreviousWeapon();

	/** [client] call the server to interact with game element */
	void Interact();
	
	//------------------------------------------------------------------------------------------------------------------
	// Weapon

	/** [server] equip weapon */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(ABPE_Weapon* WeaponToEquip);
	
	/** [server] equip overlapping weapon */
	void HandleEquipWeapon(ABPE_Weapon* WeaponToEquip);

	/** [server] Interact */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact();

	/** [server] interact with overlapping spawn pad */
	void HandleInteract();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming();
	
	/** [server] set aim walk speed */
	void OnIsAimingChanged();

	UFUNCTION()
	void OnRep_Aiming();
	
	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_OverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	/** [client] overlapping spawn pad rep handler */
	UFUNCTION()
	void OnRep_OverlappingSpawnPad(ABPE_SpawnPad* LastOverlappingSpawnPad);

	/** [client] overlapping weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon();

	/** [server and client] called when current weapon change */
	void OnCurrentWeaponChanged();

	/** [client and server] overlapping weapon handler */
	void OnSetOverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon);

	/** [client and server] on set overlapping spawn pad handler */
	void OnSetOverlappingSpawnPad(ABPE_SpawnPad* LastOverlappingSpawnPad);

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

	/** [server] drop all weapons in inventory */
	virtual void DropWeapon() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAsCurrentWeapon(ABPE_Weapon* Weapon);

	void SetAsCurrentWeapon(ABPE_Weapon* Weapon);

	/** [server] hide last weapon used */
	void HideUnusedWeapon(ABPE_Weapon* Weapon);

	/** [client] weapon inventory rep handler */
	UFUNCTION()
	void OnRep_Inventory();

	/** [server and client] weapon inventory handler */
	void OnInventoryChanged();

	//------------------------------------------------------------------------------------------------------------------
	
	void PauseGame();
	
public:

	virtual void Tick(float DeltaSeconds) override;

	/** [client] call the server to perform fire */
	virtual void StartWeaponFire() override;

	/** [client] call the server to perform stop fire */
	virtual void StopWeaponFire() override;
	
	/** [server] overlapping weapon handler */
	void SetOverlappingWeapon(ABPE_Weapon* Weapon);

	/** [server] set overlapping SpawnPad handler */
	void SetOverlappingSpawnPad(ABPE_SpawnPad* SpawnPad);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** check if current weapon is valid */
	virtual bool IsWeaponEquipped() const override;

	bool IsAiming() const { return bIsAiming; };

	/** from the camera component for clients */
	virtual FVector GetPawnViewLocation() const override;

	/** check if all player inputs are enabled or just camera movements and if it is not dead */
	bool AreGameplayInputsEnabled() const;

	//void SetPlayerController(ABPE_PlayerController* PlayerController) { PlayerControllerReference = PlayerController; }
};
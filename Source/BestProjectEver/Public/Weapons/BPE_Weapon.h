// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "BPE_Weapon.generated.h"

class USphereComponent;
class UWidgetComponent;

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
	
	/** current weapon state */
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState)
	EWeaponState CurrentState;

	/**  */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	EWeaponColorType ColorType;
	
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

public:

	//------------------------------------------------------------------------------------------------------------------
	// Reading data
	
	/** get current weapon state */
	EWeaponState GetCurrentState() const { return CurrentState; }

	/** set weapon state */
	void SetState(EWeaponState State);

	/* Called from the server and also from the client with a replicate variable*/
	void SetWidgetVisibility(bool bShowWidget);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

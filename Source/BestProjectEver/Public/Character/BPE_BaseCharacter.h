// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/HealthData.h"
#include "GameFramework/Character.h"

#include "BPE_BaseCharacter.generated.h"

class UBPE_HealthComponent;
class USoundCue;

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Enemy,
	Player
};

UCLASS()
class BESTPROJECTEVER_API ABPE_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABPE_BaseCharacter();

protected:
	
	//------------------------------------------------------------------------------------------------------------------
	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBPE_HealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintReadOnly)
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitterComponent;

	//------------------------------------------------------------------------------------------------------------------
	
	/** for AI Perception Component */
	UPROPERTY(EditDefaultsOnly, Category="AI")
	ETeam Team;
	
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;

	//------------------------------------------------------------------------------------------------------------------
	// Animation and effects
	
	/** anim instance reference */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DamageMontage;
	
protected:

	virtual void BeginPlay() override;

	//------------------------------------------------------------------------------------------------------------------
	// Character State
	
	UFUNCTION()
	virtual void HandleCharacterDeath(AActor* DamagedActor, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void HandleCharacterHealthChange(const FHealthData& HealthData);

	//------------------------------------------------------------------------------------------------------------------
	// Weapon
	
	/** drop all weapons equipped */
	virtual void DropWeapon();

	//------------------------------------------------------------------------------------------------------------------
	// Sounds And Effects

	/** play sound cue */
	virtual void PlaySound(USoundCue* Sound);

	/** play anim montage */
	virtual void PlayMontage(UAnimMontage* Montage, const FName& SectionName, float PlayRate = 1.0f);

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartWeaponFire();
	
	virtual void StopWeaponFire();

	virtual bool IsWeaponEquipped() const;

	/** actor A and actor B are from the same team */
	static bool IsFriendly(const AActor* ActorA, const AActor* ActorB);
	
	virtual UBPE_HealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	void CharacterMakeNoise(const float Loudness, const FVector NoiseLocation);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BPE_BaseCharacter.generated.h"

class UBPE_HealthComponent;
class USoundCue;

UCLASS()
class BESTPROJECTEVER_API ABPE_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABPE_BaseCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBPE_HealthComponent> HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	uint8 TeamNumber;
	
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;

	/** anim instance reference */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimInstance> AnimInstance;
	
protected:

	virtual void BeginPlay() override;
	
	virtual void StartWeaponFire();
	
	virtual void StopWeaponFire();

	//------------------------------------------------------------------------------------------------------------------
	// Sounds And Effects

	/** play sound cue */
	virtual void PlaySound(USoundCue* Sound);

	/** play anim montage */
	virtual void PlayMontage(UAnimMontage* Montage, float PlayRate = 1.0f);

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual bool IsWeaponEquipped() const;

	static bool IsFriendly(const AActor* ActorA, const AActor* ActorB);

	UFUNCTION(BlueprintCallable)
	virtual UBPE_HealthComponent* GetHealthComponent() const { return HealthComponent; }
};

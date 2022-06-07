// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BPE_BaseCharacter.generated.h"

UCLASS()
class BESTPROJECTEVER_API ABPE_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABPE_BaseCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category="AI")
	uint8 TeamNumber;
	
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;
	
protected:

	virtual void BeginPlay() override;
	
	virtual void StartWeaponFire();
	
	virtual void StopWeaponFire();

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual bool IsWeaponEquipped() const;

	static bool IsFriendly(const AActor* ActorA, const AActor* ActorB);
};

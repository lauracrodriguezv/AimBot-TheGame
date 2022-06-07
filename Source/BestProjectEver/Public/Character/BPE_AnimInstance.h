#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BPE_AnimInstance.generated.h"

class ABPE_BaseCharacter;
class ABPE_Enemy;
class ABPE_PlayerCharacter;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UBPE_AnimInstance();

protected:

	UPROPERTY(BlueprintReadOnly, Category="Character")
	TObjectPtr<ABPE_BaseCharacter> CharacterOwner;
	
	UPROPERTY(BlueprintReadOnly, Category="Character")
	TObjectPtr<ABPE_PlayerCharacter> PlayerOwner;

	UPROPERTY(BlueprintReadOnly, Category="Character")
	TObjectPtr<ABPE_Enemy> EnemyOwner;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsAccelerating : 1;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsInAir : 1;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsCrouched : 1;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsWeaponEquipped : 1;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsAiming : 1;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	uint8 bIsDead : 1;
	
public:
	
	virtual void NativeInitializeAnimation() override;

	void InitializeReferences();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateCharacterVariables();
};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BPE_AnimInstance.generated.h"

class ABPE_BaseCharacter;
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

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bIsInAir;
	
public:
	
	virtual void NativeInitializeAnimation() override;

	void InitializeReferences();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateCharacterVariables();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "UObject/Interface.h"
#include "BPE_Damagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBPE_Damagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BESTPROJECTEVER_API IBPE_Damagable
{
	GENERATED_BODY()

public:

	virtual bool CanBeDamageableWithColor(const EColorType DamageColorType) const PURE_VIRTUAL(IBPE_Damagable::CanBeDamageableWithColor, return false;);

	virtual float GetUltimateXP() const PURE_VIRTUAL(IBPE_Damagable::GetUltimateXP,  return 0.0f;);
};

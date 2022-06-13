// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BestProjectEver/ColorType.h"
#include "UObject/Interface.h"
#include "BPE_InteractWithColorType.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBPE_InteractWithColorType : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BESTPROJECTEVER_API IBPE_InteractWithColorType
{
	GENERATED_BODY()

public:
	
	virtual EColorType GetColorType() const PURE_VIRTUAL(IBPE_InteractWithColorType::GetColorType, return EColorType::Blue;)
	
	virtual void SetColorType(const EColorType NewColorType) { UpdateMeshColor(); }

	virtual void UpdateMeshColor() PURE_VIRTUAL(IBPE_InteractWithColorType::UpdateMeshColor, ;)

	virtual void OnStopInteraction() PURE_VIRTUAL(IBPE_InteractWithColorType::OnStopInteraction, ;)
};

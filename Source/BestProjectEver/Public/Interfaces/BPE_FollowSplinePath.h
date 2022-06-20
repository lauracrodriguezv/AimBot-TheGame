// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BPE_FollowSplinePath.generated.h"

UINTERFACE(MinimalAPI)
class UBPE_FollowSplinePath : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BESTPROJECTEVER_API IBPE_FollowSplinePath
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Path")
	void FollowSplinePath(const FVector& NextPointLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Path")
	void DebugActorOnSplineText(const float DistanceAlongSpline, const float PercentageAlongSpline);
};

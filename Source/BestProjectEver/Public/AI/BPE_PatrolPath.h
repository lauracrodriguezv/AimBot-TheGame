// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_PathFollowing : public AActor
{
	GENERATED_BODY()
	
public:	

	ABPE_PathFollowing();

protected:

	/** point the enemy will follow */
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent;

public:

	/** get path points from spline by index */
	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocationByIndex(const int32 Index) const;

	/** total number of spline's points */
	UFUNCTION(BlueprintCallable)
	int32 GetLastIndex() const;
};

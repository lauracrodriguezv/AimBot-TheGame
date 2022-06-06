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

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent;

public:

	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocationByIndex(const int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 GetLastIndex() const;
};

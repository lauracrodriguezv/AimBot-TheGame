// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPE_FollowSplineComponent.generated.h"

class ABPE_PathFollowing;
class USplineComponent;
class IBPE_FollowSplinePath;

/**
 * FollowSplineComponent allows an actor follows a specified spline path and it's also helpful to know when the actor
 * has reached the end of its path or gone a certain distance in their journey. For flexibility, the actor to which this
 * component is attached must to implement IBPE_FollowSplinePath to determine FollowSplinePath behavior. If the owner actor
 * is a character, orient rotation to movement must be true
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BESTPROJECTEVER_API UBPE_FollowSplineComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Path")
	uint8 bIsUsingFollowSplineComponent : 1;
	
	UPROPERTY(EditDefaultsOnly, Category="Path", meta = (EditCondition = bIsUsingFollowSplineComponent))
	uint8 bDebugActorFollowingLocation : 1;
	
	/** Forward actor's projection to get closest point on the spline */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Path", meta = (EditCondition = bIsUsingFollowSplineComponent))
	float ForwardProjectionDistance;

	/** Spline component to follow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Path", meta = (EditCondition = bIsUsingFollowSplineComponent))
	TObjectPtr<ABPE_PathFollowing> SplinePath;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Path")
	TObjectPtr<USplineComponent> SplineReference;

	/** Owner who implements IBPE_FollowSplinePath interface to perform path follow behavior */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Path")
	TObjectPtr<IBPE_FollowSplinePath> ActorFollowingPath;
	
public:
	
	UBPE_FollowSplineComponent();

protected:

	virtual void BeginPlay() override;

	/** Returns the location of the player forward projection vector depending on ForwardProjectionDistance */
	FVector GetForwardProjection() const;

	/** Update the destination of the actor following the spline path */
	FVector GetNextPointLocation() const;

	void DebugActorFollowingLocation() const;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetActorCurrentPositionOnSpline(float& OutDistanceAlongSpline, float& OutPercentageAlongSpline) const;
};

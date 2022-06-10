// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BPE_PatrolPath.h"

#include "Components/SplineComponent.h"

// Sets default values
ABPE_PathFollowing::ABPE_PathFollowing()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineCompononent"));
}

//----------------------------------------------------------------------------------------------------------------------
FVector ABPE_PathFollowing::GetWorldLocationByIndex(const int32 Index) const
{
	if(IsValid(SplineComponent) && Index >= 0 && Index <= GetNumberOfSplinePathPoints())
	{
		return SplineComponent->GetLocationAtSplinePoint(Index,ESplineCoordinateSpace::World);
	}
	return GetActorLocation();
}

//----------------------------------------------------------------------------------------------------------------------
int32 ABPE_PathFollowing::GetNumberOfSplinePathPoints() const
{
	if(IsValid(SplineComponent))
	{
		return SplineComponent->GetNumberOfSplinePoints();
	}
	return 0;
}



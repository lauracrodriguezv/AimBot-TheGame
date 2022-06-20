// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BPE_FollowSplineComponent.h"

#include "AI/BPE_PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Interfaces/BPE_FollowSplinePath.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_FollowSplineComponent::UBPE_FollowSplineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	bIsUsingFollowSplineComponent = false;
	bDebugActorFollowingLocation = false;

	ForwardProjectionDistance = 100.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_FollowSplineComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(SplinePath))
	{
		SplineReference = SplinePath->GetSplineComponent();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_FollowSplineComponent::DebugActorFollowingLocation() const
{
	DrawDebugSphere(GetWorld(), GetNextPointLocation(), 10.0f, 12.0, FColor::Green);
	DrawDebugSphere(GetWorld(), GetForwardProjection(), 10.0f, 12.0, FColor::Blue);

	float DistanceAlongSpline;
	float PercentageAlongSpline;
	GetActorCurrentPositionOnSpline(DistanceAlongSpline, PercentageAlongSpline);
	IBPE_FollowSplinePath::Execute_DebugActorOnSplineText(GetOwner(), DistanceAlongSpline, PercentageAlongSpline);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_FollowSplineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsUsingFollowSplineComponent)
	{
		if(IsValid(GetOwner()) && GetOwner()->Implements<UBPE_FollowSplinePath>())
		{
			IBPE_FollowSplinePath::Execute_FollowSplinePath(GetOwner(), GetNextPointLocation());

			if (bDebugActorFollowingLocation)
			{
				DebugActorFollowingLocation();
			}
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
FVector UBPE_FollowSplineComponent::GetNextPointLocation() const
{
	if(IsValid(SplineReference))
	{
		return SplineReference->FindLocationClosestToWorldLocation(GetForwardProjection(), ESplineCoordinateSpace::World);
	}
	return FVector();
}

//----------------------------------------------------------------------------------------------------------------------
FVector UBPE_FollowSplineComponent::GetForwardProjection() const
{
	if(IsValid(GetOwner()))
	{
		const FVector ActorForwardProjection = GetOwner()->GetActorForwardVector() * ForwardProjectionDistance;
		return GetOwner()->GetActorLocation() + ActorForwardProjection;
	}
	return FVector();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_FollowSplineComponent::GetActorCurrentPositionOnSpline(float& OutDistanceAlongSpline,
	float& OutPercentageAlongSpline) const
{
	if(IsValid(SplineReference) && IsValid(GetOwner()))
	{		
		const float ClosestInputKey = SplineReference->FindInputKeyClosestToWorldLocation(GetOwner()->GetActorLocation());
		OutDistanceAlongSpline = SplineReference->GetDistanceAlongSplineAtSplineInputKey(ClosestInputKey);

		OutPercentageAlongSpline = (OutDistanceAlongSpline / SplineReference->GetSplineLength()) * 100.0f;
	}
}
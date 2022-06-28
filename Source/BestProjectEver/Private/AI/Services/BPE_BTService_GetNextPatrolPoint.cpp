// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BPE_BTService_GetNextPatrolPoint.h"

#include "AIController.h"
#include "AI/BPE_PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_Enemy.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTService_GetNextPatrolPoint::UBPE_BTService_GetNextPatrolPoint()
{
	NodeName = "GetNextPatrolPoint";
	
	PathPatrolReferenceName = "PathPatrolReference";
	NextPatrolPointLocationName = "NextPatrolPointLocation";
	PatrolIndexName = "PatrolIndex";
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	UBlackboardComponent* BlackboardComponent = SearchData.OwnerComp.GetBlackboardComponent();

	const ABPE_PathFollowing* PathPatrolReference = nullptr;
	int32 PatrolIndex = 0;
	if(IsValid(BlackboardComponent))
	{
		PathPatrolReference = Cast<ABPE_PathFollowing>(BlackboardComponent->GetValueAsObject(PathPatrolReferenceName));
		PatrolIndex = BlackboardComponent->GetValueAsInt(PatrolIndexName);
	}
	
	SetNextPatrolPointLocation(BlackboardComponent, PathPatrolReference, PatrolIndex);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::SetNextPatrolPointLocation(UBlackboardComponent* BlackboardComponent, const ABPE_PathFollowing* PathPatrolReference, int32 PatrolIndex)
{
	if(IsValid(BlackboardComponent) && IsValid(PathPatrolReference))
	{
		const FVector NextPatrolPointLocation = PathPatrolReference->GetWorldLocationByIndex(PatrolIndex);
		
		PatrolIndex++;
		PatrolIndex = PatrolIndex < PathPatrolReference->GetNumberOfSplinePathPoints() ? PatrolIndex : 0;
		BlackboardComponent->SetValueAsInt(PatrolIndexName, PatrolIndex);
		
		BlackboardComponent->SetValueAsVector(NextPatrolPointLocationName, NextPatrolPointLocation);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}



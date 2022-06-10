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
	PatrolIndex = 0;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	BlackboardComponent = SearchData.OwnerComp.GetBlackboardComponent();
	
	const AAIController* AIController = SearchData.OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
	}
	
	GetBlackboardKeyValues();
	SetNextPatrolPointLocation();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::GetBlackboardKeyValues()
{
	if(IsValid(BlackboardComponent))
	{
		PathPatrolReference = Cast<ABPE_PathFollowing>(BlackboardComponent->GetValueAsObject(PathPatrolReferenceName));
		PatrolIndex = BlackboardComponent->GetValueAsInt(PatrolIndexName);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::SetNextPatrolPointLocation()
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



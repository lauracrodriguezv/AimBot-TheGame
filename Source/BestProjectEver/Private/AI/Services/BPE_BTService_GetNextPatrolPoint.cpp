// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BPE_BTService_GetNextPatrolPoint.h"

#include "AIController.h"
#include "AI/BPE_PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_Enemy.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTService_GetNextPatrolPoint::UBPE_BTService_GetNextPatrolPoint()
{
	NodeName = "GetDetectedTarget";
	
	PathPatrolKeyName = "PathPatrolReference";
	NextPatrolPointLocationKeyName = "NextPatrolPointLocation";
	PatrolIndexKeyName = "PatrolIndex";
	PatrolIndex = 0;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	const AAIController* AIController = SearchData.OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			EnemyOwner->SetEnemyStatus(EEnemyStatus::Patrol);
		}
	}
	
	BlackboardComponent = SearchData.OwnerComp.GetBlackboardComponent();
	GetBlackboardKeyValues();
	SetNextPatrolPointLocation();
}


//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::GetBlackboardKeyValues()
{
	if(IsValid(BlackboardComponent))
	{
		PathPatrolReference = Cast<ABPE_PathFollowing>(BlackboardComponent->GetValueAsObject(PathPatrolKeyName));
		PatrolIndex = BlackboardComponent->GetValueAsInt(PatrolIndexKeyName);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetNextPatrolPoint::SetNextPatrolPointLocation()
{
	if(IsValid(BlackboardComponent) && IsValid(PathPatrolReference))
	{
		const FVector NextPatrolPointLocation = PathPatrolReference->GetWorldLocationByIndex(PatrolIndex);
		PatrolIndex = PatrolIndex <= PathPatrolReference->GetLastIndex() ? PatrolIndex + 1 : 0;
		BlackboardComponent->SetValueAsInt(PatrolIndexKeyName, PatrolIndex);
		BlackboardComponent->SetValueAsVector(NextPatrolPointLocationKeyName, NextPatrolPointLocation);
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



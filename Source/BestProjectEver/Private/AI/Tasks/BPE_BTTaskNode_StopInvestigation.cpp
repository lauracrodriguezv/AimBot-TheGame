// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BPE_BTTaskNode_StopInvestigation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBPE_BTTaskNode_StopInvestigation::UBPE_BTTaskNode_StopInvestigation()
{
	NodeName = "StopInvestigation";
	bNotifyTick = false;
	IsInvestigatingKey = "bIsInvestigating";
}

//----------------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBPE_BTTaskNode_StopInvestigation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsBool(IsInvestigatingKey, false);

	return EBTNodeResult::Succeeded;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTTaskNode_StopInvestigation::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

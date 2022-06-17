// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BPE_BTDecorator_IsInvestigating.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTDecorator_IsInvestigating::UBPE_BTDecorator_IsInvestigating()
{
	NodeName = "IsInvestigating";
	NotifyObserver = EBTBlackboardRestart::ResultChange;
	
	FlowAbortMode = EBTFlowAbortMode::Both;
}

//----------------------------------------------------------------------------------------------------------------------
bool UBPE_BTDecorator_IsInvestigating::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		const ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			return EnemyOwner->GetEnemyStatus() == EEnemyStatus::Investigating;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_IsInvestigating::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_IsInvestigating::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

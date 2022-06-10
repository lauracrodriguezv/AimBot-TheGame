// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BPE_BTTask_SetEnemyStatus.h"
#include "AIController.h"
#include "Character/BPE_Enemy.h"

UBPE_BTTask_SetEnemyStatus::UBPE_BTTask_SetEnemyStatus()
{
	NodeName = "SetEnemyStatus";
	bNotifyTick = false;
}

//----------------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBPE_BTTask_SetEnemyStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			EnemyOwner->SetEnemyStatus(NewEnemyStatus);
			return  EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTTask_SetEnemyStatus::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTTask_SetEnemyStatus::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}


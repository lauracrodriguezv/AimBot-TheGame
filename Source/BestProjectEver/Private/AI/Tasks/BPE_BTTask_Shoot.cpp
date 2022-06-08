// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BPE_BTTask_Shoot.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"

UBPE_BTTask_Shoot::UBPE_BTTask_Shoot()
{
	NodeName = "Shoot";
	bNotifyTick = false;
}

//----------------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBPE_BTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			EnemyOwner->StartWeaponFire();
			return  EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTTask_Shoot::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

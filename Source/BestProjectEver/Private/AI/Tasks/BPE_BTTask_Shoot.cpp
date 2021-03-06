// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BPE_BTTask_Shoot.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBPE_BTTask_Shoot::UBPE_BTTask_Shoot()
{
	NodeName = "Shoot";
	bNotifyTick = false;

	TargetLocationName = "TargetLocation";
}

//----------------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBPE_BTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(IsValid(AIController) && IsValid(BlackboardComponent))
	{
		ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			const FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetLocationName);
			EnemyOwner->SetTargetViewLocation(TargetLocation);
			
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

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTTask_Shoot::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

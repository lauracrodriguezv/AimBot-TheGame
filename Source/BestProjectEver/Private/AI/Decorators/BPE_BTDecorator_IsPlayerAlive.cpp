// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BPE_BTDecorator_IsPlayerAlive.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_PlayerCharacter.h"
#include "Components/BPE_HealthComponent.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTDecorator_IsPlayerAlive::UBPE_BTDecorator_IsPlayerAlive()
{
	NodeName = "IsPlayerAlive";
	NotifyObserver = EBTBlackboardRestart::ResultChange;
	
	FlowAbortMode = EBTFlowAbortMode::Both;
	SetIsInversed(false);
	
	TargetReferenceName = "TargetReference";	
}

//----------------------------------------------------------------------------------------------------------------------
bool UBPE_BTDecorator_IsPlayerAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(IsValid(BlackboardComponent))
	{
		const UObject* TargetReference = BlackboardComponent->GetValueAsObject(TargetReferenceName);
		const ABPE_PlayerCharacter* TargetPlayer = Cast<ABPE_PlayerCharacter>(TargetReference);
		if(IsValid(TargetPlayer))
		{
			return !TargetPlayer->GetHealthComponent()->IsDead();
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_IsPlayerAlive::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_IsPlayerAlive::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

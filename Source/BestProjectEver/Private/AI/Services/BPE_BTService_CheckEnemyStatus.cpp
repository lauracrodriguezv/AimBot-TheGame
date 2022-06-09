// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BPE_BTService_CheckEnemyStatus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_Enemy.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionComponent.h"

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_CheckEnemyStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner) && EnemyOwner->GetEnemyStatus() != EEnemyStatus::Investigating)
		{
			TArray<AActor*> PerceivedActors;
			AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedActors);

			if(!PerceivedActors.IsEmpty())
			{
				EnemyOwner->SetEnemyStatus(EEnemyStatus::Investigating);
			}	
		}
	}	
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_CheckEnemyStatus::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_CheckEnemyStatus::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

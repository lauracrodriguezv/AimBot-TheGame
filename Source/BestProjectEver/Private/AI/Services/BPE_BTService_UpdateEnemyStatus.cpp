// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BPE_BTService_UpdateEnemyStatus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_Enemy.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionComponent.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTService_UpdateEnemyStatus::UBPE_BTService_UpdateEnemyStatus()
{
	NodeName = "UpdateEnemyStatus";
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_UpdateEnemyStatus::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	AIController = SearchData.OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_UpdateEnemyStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(IsValid(AIController) && IsValid(EnemyOwner))
	{
		const bool bIsInvestigating = EnemyOwner->GetEnemyStatus() == EEnemyStatus::Investigating;
		if(!bIsInvestigating)
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
void UBPE_BTService_UpdateEnemyStatus::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_UpdateEnemyStatus::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

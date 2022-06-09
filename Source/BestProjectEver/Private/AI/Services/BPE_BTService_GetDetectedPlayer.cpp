


#include "AI/Services/BPE_BTService_GetDetectedPlayer.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_BTService_GetDetectedTarget::UBPE_BTService_GetDetectedTarget()
{
	NodeName = "GetDetectedTarget";
	
	TargetReferenceKeyName = "TargetReference";
	TargetLocationKeyName = "TargetLocation";
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(IsValid(EnemyOwner))
	{
		TArray<AActor*> PerceivedActors;
		AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
		
		if(PerceivedActors.IsEmpty())
		{
			AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedActors);
			if(PerceivedActors.IsEmpty())
			{
				ResetBlackboardKeysValues();
			}
			else
			{
				SetBlackboardKeyValues(PerceivedActors);
				EnemyOwner->SetEnemyStatus(EEnemyStatus::Investigating);
			}
		}
		else
		{
			SetBlackboardKeyValues(PerceivedActors);
			EnemyOwner->SetEnemyStatus(EEnemyStatus::Combat);
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	AIController = SearchData.OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		BlackboardComponent = SearchData.OwnerComp.GetBlackboardComponent();
		EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::SetBlackboardKeyValues(const TArray<AActor*>& PerceivedActors)
{
	for (AActor* PerceivedActor : PerceivedActors)
	{
		if(IsValid(PerceivedActor) && IsValid(BlackboardComponent))
		{
			BlackboardComponent->SetValueAsObject(TargetReferenceKeyName, PerceivedActor);
			BlackboardComponent->SetValueAsVector(TargetLocationKeyName, PerceivedActor->GetActorLocation());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::ResetBlackboardKeysValues()
{
	if(IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsObject(TargetReferenceKeyName, nullptr);
		BlackboardComponent->SetValueAsVector(TargetLocationKeyName, FVector());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

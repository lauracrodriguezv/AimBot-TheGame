


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
	
	TargetReferenceName = "TargetReference";
	TargetLocationName = "TargetLocation";
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	const ABPE_Enemy* EnemyOwner = nullptr;
	if(IsValid(AIController))
	{
		EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
	}
	
	if(IsValid(EnemyOwner))
	{
		TArray<AActor*> PerceivedActors;
		
		AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
		if(PerceivedActors.IsEmpty())
		{			
			AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedActors);
			if(PerceivedActors.IsEmpty())
			{
				ResetBlackboardKeysValues(BlackboardComponent);
			}
			else
			{
				SetBlackboardKeyValues(BlackboardComponent, PerceivedActors);
			}
		}
		else
		{
			SetBlackboardKeyValues(BlackboardComponent, PerceivedActors);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::SetBlackboardKeyValues(UBlackboardComponent* BlackboardComponent, const TArray<AActor*>& PerceivedActors) const
{
	for (AActor* PerceivedActor : PerceivedActors)
	{
		if(IsValid(PerceivedActor) && IsValid(BlackboardComponent))
		{
			BlackboardComponent->SetValueAsObject(TargetReferenceName, PerceivedActor);
			BlackboardComponent->SetValueAsVector(TargetLocationName, PerceivedActor->GetActorLocation());
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTService_GetDetectedTarget::ResetBlackboardKeysValues(UBlackboardComponent* BlackboardComponent) const
{
	if(IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsObject(TargetReferenceName, nullptr);
		BlackboardComponent->SetValueAsVector(TargetLocationName, FVector());
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

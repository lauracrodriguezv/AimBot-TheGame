


#include "AI/Services/BPE_BTService_GetDetectedPlayer.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BPE_HealthComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
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
	
	TArray<AActor*> PerceivedTargets;

	if(!TrySetSeenTarget(AIController, PerceivedTargets, BlackboardComponent))
	{
		if(!TryGetDamageCauserLocation(AIController, PerceivedTargets, BlackboardComponent))
		{
			if(!TryGetHeardLocation(AIController, PerceivedTargets, BlackboardComponent))
			{
				ResetBlackboardKeysValues(BlackboardComponent);
			}	
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
bool UBPE_BTService_GetDetectedTarget::TrySetSeenTarget(AAIController* AIController, TArray<AActor*>& PerceivedTargets,
	UBlackboardComponent* BlackboardComponent)
{
	if(IsValid(AIController))
	{
		AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedTargets);
		if(!PerceivedTargets.IsEmpty())
		{
			for (AActor* PerceivedActor : PerceivedTargets)
			{
				if(IsValid(PerceivedActor) && IsValid(BlackboardComponent))
				{
					BlackboardComponent->SetValueAsObject(TargetReferenceName, PerceivedActor);
					BlackboardComponent->SetValueAsVector(TargetLocationName, PerceivedActor->GetActorLocation());
					break;
				}
			}
			return true;
		}	
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool UBPE_BTService_GetDetectedTarget::TryGetHeardLocation(AAIController* AIController, TArray<AActor*>& PerceivedTargets,
	UBlackboardComponent* BlackboardComponent)
{
	if(IsValid(AIController))
	{
		AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedTargets);
		if(!PerceivedTargets.IsEmpty())
		{
			for (AActor* PerceivedActor : PerceivedTargets)
			{
				if(IsValid(PerceivedActor) && IsValid(BlackboardComponent))
				{
					BlackboardComponent->SetValueAsVector(TargetLocationName, PerceivedActor->GetActorLocation());
					break;
				}
			}
			return true;
		}	
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool UBPE_BTService_GetDetectedTarget::TryGetDamageCauserLocation(AAIController* AIController, TArray<AActor*>& PerceivedTargets,
	UBlackboardComponent* BlackboardComponent)
{
	if(IsValid(AIController))
	{
		AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Damage::StaticClass(), PerceivedTargets);
		if(!PerceivedTargets.IsEmpty())
		{
			for (AActor* PerceivedActor : PerceivedTargets)
			{
				if(IsValid(PerceivedActor) && IsValid(BlackboardComponent))
				{
					const ABPE_Enemy* Enemy = Cast<ABPE_Enemy>(AIController->GetPawn());
					if(IsValid(Enemy))
					{
						AActor* DamageCauser = Enemy->GetHealthComponent()->GetDamageCauser();
						if(IsValid(DamageCauser))
						{
							BlackboardComponent->SetValueAsObject(TargetReferenceName, DamageCauser);
							BlackboardComponent->SetValueAsVector(TargetLocationName, DamageCauser->GetActorLocation());	
						}
					}
					break;
				}
			}
			return true;
		}	
	}
	return false;
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

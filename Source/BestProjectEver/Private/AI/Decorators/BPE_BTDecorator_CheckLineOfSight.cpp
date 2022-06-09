// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BPE_BTDecorator_CheckLineOfSight.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BPE_PlayerCharacter.h"

UBPE_BTDecorator_CheckLineOfSight::UBPE_BTDecorator_CheckLineOfSight()
{
	NodeName = "HasLineOfSight";
	FlowAbortMode = EBTFlowAbortMode::Both;
	SetIsInversed(true);

	TargetReferenceName = "TargetReference";
	TargetLocationName = "TargetLocation";
}

bool UBPE_BTDecorator_CheckLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(IsValid(AIController))
	{
		ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		if(IsValid(EnemyOwner))
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			EnemyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector TraceStart = EyeLocation;

			UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
			FVector TraceEnd = BlackboardComponent->GetValueAsVector(TargetLocationName);
			
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(EnemyOwner);
			QueryParams.AddIgnoredComponent(EnemyOwner->GetMesh());
			QueryParams.bTraceComplex = true;

			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

			if(HitResult.bBlockingHit)
			{
				ABPE_PlayerCharacter* TargetReference = Cast<ABPE_PlayerCharacter>(BlackboardComponent->GetValueAsObject(TargetReferenceName));
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, true, 2.0);
				return Cast<ABPE_PlayerCharacter>(HitResult.GetActor()) == TargetReference;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_CheckLineOfSight::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_CheckLineOfSight::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

void UBPE_BTDecorator_CheckLineOfSight::TraceFromEnemySight(ABPE_Enemy* Enemy, FHitResult& HitResult)
{
}


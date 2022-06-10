// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BPE_BTDecorator_HasLineOfSight.h"

#include "Character/BPE_Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBPE_BTDecorator_HasLineOfSight::UBPE_BTDecorator_HasLineOfSight()
{
	NodeName = "HasLineOfSight";
	FlowAbortMode = EBTFlowAbortMode::Both;

	/** if the enemy does not have line of sight, search for a better combat position*/ 
	SetIsInversed(true);

	TargetReferenceName = "TargetReference";
	TargetLocationName = "TargetLocation";
}

bool UBPE_BTDecorator_HasLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	if(IsValid(AIController) && IsValid(BlackboardComponent))
	{
		const ABPE_Enemy* EnemyOwner = Cast<ABPE_Enemy>(AIController->GetPawn());
		const FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetLocationName);
		FHitResult HitResult;
		
		TraceFromEnemyToTarget(EnemyOwner, TargetLocation, HitResult);
		
		if(HitResult.bBlockingHit)
		{
			const AActor* TargetReference = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetReferenceName));
			const AActor* ActorHit = Cast<AActor>(HitResult.GetActor());
			return ActorHit == TargetReference;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_HasLineOfSight::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_HasLineOfSight::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_BTDecorator_HasLineOfSight::TraceFromEnemyToTarget(const ABPE_Enemy* Enemy, FVector TargetLocation, FHitResult& OutHitResult) const 
{
	if(IsValid(Enemy))
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		Enemy->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		const FVector TraceStart = EyeLocation;
		const FVector TraceEnd = TargetLocation;
			
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Enemy);
		QueryParams.bTraceComplex = true;
	
		GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);	
	}
}


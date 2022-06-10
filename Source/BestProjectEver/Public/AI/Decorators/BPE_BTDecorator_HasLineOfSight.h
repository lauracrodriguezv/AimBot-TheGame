// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BPE_BTDecorator_HasLineOfSight.generated.h"

class ABPE_Enemy;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTDecorator_HasLineOfSight : public UBTDecorator
{
	GENERATED_BODY()

protected:

	/** target detected by controller */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetReferenceName;

	/** target detected by controller location */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetLocationName;

public:

	UBPE_BTDecorator_HasLineOfSight();
	
protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	void TraceFromEnemyToTarget(const ABPE_Enemy* Enemy, FVector TargetLocation, FHitResult& OutHitResult) const;

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

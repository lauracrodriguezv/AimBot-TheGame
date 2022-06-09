// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BPE_BTDecorator_CheckLineOfSight.generated.h"

class ABPE_Enemy;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTDecorator_CheckLineOfSight : public UBTDecorator
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetReferenceName;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetLocationName;

	
public:

	UBPE_BTDecorator_CheckLineOfSight();
	
protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	void TraceFromEnemySight(ABPE_Enemy* Enemy, FHitResult& HitResult);
};

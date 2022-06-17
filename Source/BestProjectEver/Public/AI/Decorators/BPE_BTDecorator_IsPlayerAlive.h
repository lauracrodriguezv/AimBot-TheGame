// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BPE_BTDecorator_IsPlayerAlive.generated.h"

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTDecorator_IsPlayerAlive : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

protected:

	/** target detected by controller */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetReferenceName;
	
public:

	UBPE_BTDecorator_IsPlayerAlive();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BPE_BTTask_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	/** target detected by controller location */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetLocationName;
	
public:

	UBPE_BTTask_Shoot();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

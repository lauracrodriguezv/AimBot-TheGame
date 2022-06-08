// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BPE_BTTaskNode_StopInvestigation.generated.h"

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTTaskNode_StopInvestigation : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBPE_BTTaskNode_StopInvestigation();

	UPROPERTY(EditDefaultsOnly, Category="Enemy State")
	FName IsInvestigatingKey;
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
};

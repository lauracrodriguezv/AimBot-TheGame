// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BestProjectEver/EnemyDefinitions.h"
#include "BPE_BTTask_SetEnemyStatus.generated.h"

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTTask_SetEnemyStatus : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBPE_BTTask_SetEnemyStatus();

	/** current enemy´s behavior */
	UPROPERTY(EditAnywhere, Category="Enemy State")
	EEnemyStatus NewEnemyStatus;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BPE_BTService_UpdateEnemyStatus.generated.h"

class ABPE_Enemy;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTService_UpdateEnemyStatus : public UBTService
{
	GENERATED_BODY()

protected:

	TObjectPtr<AAIController> AIController;

	TObjectPtr<ABPE_Enemy> EnemyOwner;
	
public:

	UBPE_BTService_UpdateEnemyStatus();
	
protected:

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

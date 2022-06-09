// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BPE_BTService_GetNextPatrolPoint.generated.h"


class ABPE_PathFollowing;
class ABPE_Enemy;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTService_GetNextPatrolPoint : public UBTService
{
	GENERATED_BODY()

protected:

	int PatrolIndex;
	
	UPROPERTY(EditAnywhere, Category="Target")
	FName PathPatrolKeyName;

	UPROPERTY(EditAnywhere, Category="Target")
	FName NextPatrolPointLocationKeyName;

	UPROPERTY(EditAnywhere, Category="Target")
	FName PatrolIndexKeyName;

	TObjectPtr<ABPE_PathFollowing> PathPatrolReference;

	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	TObjectPtr<ABPE_Enemy> EnemyOwner;

public:

	UBPE_BTService_GetNextPatrolPoint();
	
protected:

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	void GetBlackboardKeyValues();

	void SetNextPatrolPointLocation();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

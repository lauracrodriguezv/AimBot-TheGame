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

	/** next point index of path patrol */
	int PatrolIndex;

	/** path key enemy follows */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName PathPatrolReferenceName;

	/** next point location key of path patrol */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName NextPatrolPointLocationName;

	/** next point index key of path patrol */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName PatrolIndexName;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_PathFollowing> PathPatrolReference;

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_Enemy> EnemyOwner;

public:

	UBPE_BTService_GetNextPatrolPoint();
	
protected:

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	void GetBlackboardKeyValues();

	void SetNextPatrolPointLocation();

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

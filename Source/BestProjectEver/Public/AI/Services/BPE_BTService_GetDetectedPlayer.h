// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BPE_BTService_GetDetectedPlayer.generated.h" 

class UAISense;
class ABPE_Enemy;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_BTService_GetDetectedTarget : public UBTService
{
	GENERATED_BODY()
	
protected:

	/** target detected by controller */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetReferenceName;

	/** target detected by controller location */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FName TargetLocationName;

	UPROPERTY(Transient)
	TObjectPtr<AAIController> AIController;

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(Transient)
	TObjectPtr<ABPE_Enemy> EnemyOwner;
	
public:

	UBPE_BTService_GetDetectedTarget();
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	/** update blackboard keys */
	void SetBlackboardKeyValues(const TArray<AActor*>& PerceivedActors);

	/** reset blackboard keys */
	void ResetBlackboardKeysValues();

public:
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

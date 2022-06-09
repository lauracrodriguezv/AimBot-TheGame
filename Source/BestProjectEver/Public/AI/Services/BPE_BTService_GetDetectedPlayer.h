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

public:

	UBPE_BTService_GetDetectedTarget();

protected:
	
	UPROPERTY(EditAnywhere, Category="Target")
	FName TargetReferenceKeyName;

	UPROPERTY(EditAnywhere, Category="Target")
	FName TargetLocationKeyName;

	TObjectPtr<AAIController> AIController;
	
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	TObjectPtr<ABPE_Enemy> EnemyOwner;
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	void SetBlackboardKeyValues(const TArray<AActor*>& PerceivedActors);

	void ResetBlackboardKeysValues();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

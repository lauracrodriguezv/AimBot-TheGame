// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BPE_AIController.generated.h"

class UBehaviorTree;
class ABPE_Enemy;
class UBlackboardComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_AIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<ABPE_Enemy> CharacterControlled;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UBehaviorTree> EnemyBehaviourTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UBlackboardComponent> BlackboardReference;
	
public:

	ABPE_AIController();
	
protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	void UpdateBlackboardKeys();
};

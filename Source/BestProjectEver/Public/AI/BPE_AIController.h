// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BPE_AIController.generated.h"

class UBehaviorTree;
class ABPE_Enemy;
class UBlackboardComponent;
class UAIPerceptionComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_AIController : public AAIController
{
	GENERATED_BODY()

protected:

	/** Minimum time before enemy is destroyed when dies */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameMode", meta=(ClampMin=0.0f))
	float DestroyDelay;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	FName PathPatrolReferenceName;

	/** from where the enemy hear a noise */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	FName InvestigatingLocationName;

	/** from where the enemy hear a noise */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	FName PlayerLocationName;
	
	UPROPERTY(BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<ABPE_Enemy> CharacterControlled;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UBehaviorTree> EnemyBehaviourTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UBlackboardComponent> BlackboardReference;

	/** path enemy follows on patrol state */
	UPROPERTY(BlueprintReadOnly, Category="Enemy Controller")
	TObjectPtr<UObject> PathFollowing;

	/** Handle for efficient management of destroy timer */
	FTimerHandle TimerHandle_Destroy;
	
public:

	ABPE_AIController();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMatchStateChanged(const FName MatchState);
	
	/** [server] State entered when inactive */
	virtual void BeginInactiveState() override;

	void BeginCooldownState();

	UFUNCTION()
	void DestroyEnemy();

	void InitializeReferences();

	void SetPathBlackboardKey();

	/** compare team attitude between perceived actor and CharacterControlled */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnAIPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};

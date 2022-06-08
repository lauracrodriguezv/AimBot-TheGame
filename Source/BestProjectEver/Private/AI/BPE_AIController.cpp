// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BPE_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BPE_Enemy.h"
#include "Character/BPE_BaseCharacter.h"
#include "AI/BPE_PatrolPath.h"

ABPE_AIController::ABPE_AIController()
{
	PathPatrolReferenceName = "PathPatrolReference";
	SetGenericTeamId(FGenericTeamId(1));
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_AIController::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_AIController::InitializeReferences()
{
	if(IsValid(EnemyBehaviourTree))
	{
		RunBehaviorTree(EnemyBehaviourTree);
	}

	CharacterControlled = Cast<ABPE_Enemy>(K2_GetPawn());
	if(IsValid(CharacterControlled))
	{
		BlackboardReference = UAIBlueprintHelperLibrary::GetBlackboard(this);
		if(IsValid(EnemyBehaviourTree))
		{
			UpdateBlackboardKeys();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_AIController::UpdateBlackboardKeys()
{
	PathFollowing = CharacterControlled->GetPathFollowing();
	if(IsValid(PathFollowing))
	{
		BlackboardReference->SetValueAsObject(PathPatrolReferenceName, PathFollowing);
	}
}

//----------------------------------------------------------------------------------------------------------------------
ETeamAttitude::Type ABPE_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return ABPE_BaseCharacter::IsFriendly(GetPawn(), &Other) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

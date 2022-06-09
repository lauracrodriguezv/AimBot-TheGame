// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BPE_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BPE_Enemy.h"
#include "Character/BPE_BaseCharacter.h"
#include "AI/BPE_PatrolPath.h"
#include "Perception/AIPerceptionComponent.h"

ABPE_AIController::ABPE_AIController()
{
	PathPatrolReferenceName = "PathPatrolReference";
	InvestigatingLocationName = "InvestigatingLocation";
	PlayerLocationName = "PlayerReferenceLocation";
	SetGenericTeamId(FGenericTeamId(1));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
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
	BlackboardReference = UAIBlueprintHelperLibrary::GetBlackboard(this);

	SetPathBlackboardKey();

	if(IsValid(AIPerceptionComponent))
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABPE_AIController::OnAIPerceptionUpdated);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_AIController::SetPathBlackboardKey()
{
	if(IsValid(CharacterControlled) && IsValid(BlackboardReference))
	{
		PathFollowing = CharacterControlled->GetPathFollowing();
		if(IsValid(PathFollowing))
		{
			BlackboardReference->SetValueAsObject(PathPatrolReferenceName, PathFollowing);
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
ETeamAttitude::Type ABPE_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return ABPE_BaseCharacter::IsFriendly(GetPawn(), &Other) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_AIController::OnAIPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(IsValid(BlackboardReference) && IsValid(CharacterControlled))
	{
		BlackboardReference->SetValueAsVector(InvestigatingLocationName, Stimulus.StimulusLocation);
		
		const FVector PlayerLocation = BlackboardReference->GetValueAsVector(PlayerLocationName);
		CharacterControlled->SetTargetViewLocation(PlayerLocation);
	}

	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_AnimInstance.h"

#include "Character/BPE_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

//----------------------------------------------------------------------------------------------------------------------
UBPE_AnimInstance::UBPE_AnimInstance()
{
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_AnimInstance::InitializeReferences()
{
	CharacterOwner = Cast<ABPE_BaseCharacter>(TryGetPawnOwner());
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateCharacterVariables();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_AnimInstance::UpdateCharacterVariables()
{
	if(IsValid(CharacterOwner))
	{
		FVector Velocity = CharacterOwner->GetVelocity();
		Velocity.Z = 0.0f;
		Speed = Velocity.Size();

		bIsInAir = CharacterOwner->GetCharacterMovement()->IsFalling();
		bIsAccelerating = CharacterOwner->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsCrouched = CharacterOwner->GetCharacterMovement()->IsCrouching();
	}
}

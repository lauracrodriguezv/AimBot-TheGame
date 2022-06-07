// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_BaseCharacter.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_BaseCharacter::ABPE_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponSocketName = "SCK_Weapon";
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StartWeaponFire()
{
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StopWeaponFire()
{
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_BaseCharacter::IsWeaponEquipped() const 
{
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
bool ABPE_BaseCharacter::IsFriendly(const AActor* ActorA, const AActor* ActorB)
{
	if(IsValid(ActorA) && IsValid(ActorB))
	{
		const ABPE_BaseCharacter* CharacterA = Cast<ABPE_BaseCharacter>(ActorA);
		const ABPE_BaseCharacter* CharacterB = Cast<ABPE_BaseCharacter>(ActorB);

		if(IsValid(CharacterA) && IsValid(CharacterB))
		{
			return CharacterA->TeamNumber == CharacterB->TeamNumber;
		}

		return true;
	}
	return true;
}


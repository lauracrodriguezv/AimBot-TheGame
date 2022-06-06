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
void ABPE_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_BaseCharacter::IsWeaponEquipped() const 
{
	return true;
}


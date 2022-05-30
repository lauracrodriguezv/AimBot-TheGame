// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_BaseCharacter.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_BaseCharacter::ABPE_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


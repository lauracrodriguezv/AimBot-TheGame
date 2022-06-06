// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"

ABPE_Enemy::ABPE_Enemy()
{
	EnemySpeedMap.Add(EEnemyStatus::Patrol, 300.0);
	EnemySpeedMap.Add(EEnemyStatus::Combat, 600.0);
	EnemySpeedMap.Add(EEnemyStatus::Investigating, 500.0);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SpawnWeapon()
{
	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::StartWeaponFire()
{
	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::StopWeaponFire()
{
	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SetEnemyStatus(EEnemyStatus NewEnemyStatus)
{
	if(EnemySpeedMap.Contains(NewEnemyStatus))
	{
		UE_LOG(LogTemp, Warning, TEXT("Current status %d"), NewEnemyStatus)
		const float Speed = EnemySpeedMap[NewEnemyStatus];
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}

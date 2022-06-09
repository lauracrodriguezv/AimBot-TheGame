// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/BPE_Weapon.h"

ABPE_Enemy::ABPE_Enemy()
{
	EnemySpeedMap.Add(EEnemyStatus::Patrol, 300.0);
	EnemySpeedMap.Add(EEnemyStatus::Combat, 600.0);
	EnemySpeedMap.Add(EEnemyStatus::Investigating, 500.0);

	TeamNumber = 0;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SpawnWeapon()
{
	if(IsValid(WeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<ABPE_Weapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if(IsValid(CurrentWeapon))
		{
			CurrentWeapon->OnPickup(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::StartWeaponFire()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartFire();	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::StopWeaponFire()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopFire();
	}
}

//----------------------------------------------------------------------------------------------------------------------	
void ABPE_Enemy::OnSetEnemyStatus(EEnemyStatus NewEnemyStatus)
{
	if(EnemySpeedMap.Contains(NewEnemyStatus))
	{
		const float Speed = EnemySpeedMap[NewEnemyStatus];
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SetEnemyStatus(EEnemyStatus NewEnemyStatus)
{
	EnemyStatus = NewEnemyStatus;
	OnSetEnemyStatus(NewEnemyStatus);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Enemy::IsWeaponEquipped() const
{
	return IsValid(CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
FRotator ABPE_Enemy::GetViewRotation() const
{
	/*if(IsValid(CurrentWeapon))
	{
		const FTransform WeaponSocketTransform = CurrentWeapon->GetMuzzleSocketTransform();
		const FVector WeaponToPlayer = TargetViewLocation - WeaponSocketTransform.GetLocation();
		return FRotator(WeaponToPlayer.Rotation());
	}*/
	return Super::GetViewRotation();
	
}
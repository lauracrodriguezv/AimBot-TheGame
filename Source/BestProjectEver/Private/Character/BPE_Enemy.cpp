// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/BPE_Weapon.h"

ABPE_Enemy::ABPE_Enemy()
{	
	EnemySpeedMap.Add(EEnemyStatus::Patrol, 300.0);
	EnemySpeedMap.Add(EEnemyStatus::Combat, 600.0);
	EnemySpeedMap.Add(EEnemyStatus::Investigating, 500.0);

	Team = ETeam::Enemy;
	ColorType = EColorType::Red;

	ImpulseOnStopInteraction = 1000.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateMeshColor();
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
			CurrentWeapon->SetColorType(GetColorType());
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
void ABPE_Enemy::OnRep_ColorType()
{
	UpdateMeshColor();
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
	if(IsValid(CurrentWeapon))
	{
		const FTransform WeaponSocketTransform = CurrentWeapon->GetMuzzleSocketTransform();
		const FVector WeaponToPlayer = TargetViewLocation - WeaponSocketTransform.GetLocation();
		
		return FRotator(WeaponToPlayer.Rotation());
	}
	return Super::GetViewRotation();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SetColorType(const EColorType NewColorType)
{
	ColorType = NewColorType;
	IBPE_InteractWithColorType::SetColorType(NewColorType);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::UpdateMeshColor()
{
	if(IsValid(GetMesh()) && MaterialColor.Contains(ColorType))
	{
		UMaterialInstanceDynamic* EnemyMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0,GetMesh()->GetMaterial(0));
		if(IsValid(EnemyMaterial))
		{
			EnemyMaterial->SetVectorParameterValue("MainColor", FLinearColor(MaterialColor[ColorType]));	
		}

		if(IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetColorType(GetColorType());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::Multicast_UpdateMeshPhysics_Implementation()
{
	if(IsValid(GetMesh()))
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::OnStopInteraction()
{
	if(IsValid(GetMesh()))
	{
		Multicast_UpdateMeshPhysics();
		GetMesh()->AddImpulse(FMath::VRand() * ImpulseOnStopInteraction, NAME_None, true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPE_Enemy, ColorType);
}

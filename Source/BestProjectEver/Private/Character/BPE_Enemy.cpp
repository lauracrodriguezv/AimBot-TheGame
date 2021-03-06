// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_Enemy.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BPE_FollowSplineComponent.h"
#include "Components/BPE_HealthComponent.h"
#include "Engine/TextRenderActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/BPE_Weapon.h"
#include "Core/GameModes/BPE_LobbyGameMode.h"
#include "Components/WidgetComponent.h"
#include "HUD/Widgets/BPE_EnemyHealthBar.h"
#include "HUD/Widgets/BPE_PromptWidget.h"

ABPE_Enemy::ABPE_Enemy()
{
	Team = ETeam::Enemy;
	DefaultColorType = EColorType::Red;

	ImpulseOnStopInteraction = 1000.0f;
	UpdateMaterialOnEnemyStatus();
	
	DestroyDelay = 2.0f;
	
	FollowSplineComponent = CreateDefaultSubobject<UBPE_FollowSplineComponent>(TEXT("FollowSplineComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	XPValue = 10.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OnColorTypeChanged(DefaultColorType);
	UpdateMaterialOnEnemyStatus();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		SpawnWeapon();
	}

	if(IsValid(HealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthBarWidget = Cast<UBPE_EnemyHealthBar>(HealthWidgetComponent->GetUserWidgetObject());
		HealthComponent->OnHealthChangeDelegate.AddDynamic(HealthBarWidget, &UBPE_EnemyHealthBar::UpdateHealthDisplay);
	}

	if(IsValid(HealthBarWidget) && BodyMaterialColor.Contains(DefaultColorType))
	{
		HealthBarWidget->SetHealthBarColor(BodyMaterialColor[DefaultColorType]);
	}
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
			CurrentWeapon->SetDefaultColorType(DefaultColorType);
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
void ABPE_Enemy::OnSetEnemyStatus()
{
	if(ParameterOnNewStatus.Contains(EnemyStatus))
	{
		const float Speed = ParameterOnNewStatus[EnemyStatus].MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	UpdateMaterialOnEnemyStatus();

	if(IsValid(HealthBarWidget))
	{
		const ESlateVisibility HealthBarVisibility = EnemyStatus == EEnemyStatus::Combat ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		HealthBarWidget->SetVisibility(HealthBarVisibility);
	}
}

//----------------------------------------------------------------------------------------------------------------------	
void ABPE_Enemy::UpdateMaterialOnEnemyStatus()
{
	if(IsValid(GetMesh()))
	{
		UMaterialInstanceDynamic* EyesMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(1,GetMesh()->GetMaterial(1));
		UMaterialInstanceDynamic* BodyMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0,GetMesh()->GetMaterial(0));

		if(IsValid(EyesMaterial) && IsValid(BodyMaterial) && ParameterOnNewStatus.Contains(EnemyStatus))
		{
			EyesMaterial->SetVectorParameterValue("Color Eyes", FLinearColor(ParameterOnNewStatus[EnemyStatus].EmissiveColor));
			EyesMaterial->SetScalarParameterValue("AnimationMultiplier", ParameterOnNewStatus[EnemyStatus].AnimationEmissiveMultiplier);
			EyesMaterial->SetVectorParameterValue("Color Visor", FLinearColor(ParameterOnNewStatus[EnemyStatus].ColorVisor));
			
			BodyMaterial->SetVectorParameterValue("Emissive", FLinearColor(ParameterOnNewStatus[EnemyStatus].EmissiveColor));
			BodyMaterial->SetScalarParameterValue("EmissiveIntensity", ParameterOnNewStatus[EnemyStatus].EmissiveIntensity);
		}
	}	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::OnRep_EnemyStatus()
{
	OnSetEnemyStatus();
}

//----------------------------------------------------------------------------------------------------------------------	
void ABPE_Enemy::OnRep_ColorType()
{
	OnColorTypeChanged(DefaultColorType);
}

//----------------------------------------------------------------------------------------------------------------------	
void ABPE_Enemy::FollowSplinePath_Implementation(const FVector& NextPointLocation)
{
	if(IsValid(GetMovementComponent()))
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NextPointLocation);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(LookAtRotation), GetMovementComponent()->GetMaxSpeed(), true);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SetEnemyStatus(EEnemyStatus NewEnemyStatus)
{
	EnemyStatus = NewEnemyStatus;
	OnSetEnemyStatus();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Enemy::IsWeaponEquipped() const
{
	return IsValid(CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::HandleCharacterDeath(AActor* DamagedActor, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::HandleCharacterDeath(DamagedActor, InstigatedBy, DamageCauser);

	if(GetWorld()->GetAuthGameMode<AGameModeBase>()->IsA(ABPE_LobbyGameMode::StaticClass()))
	{
		OnStopInteraction();
	}
	else
	{
		DropWeapon();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::DropWeapon()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->OnDropped();
		CurrentWeapon = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
FRotator ABPE_Enemy::GetViewRotation() const
{
	if(IsValid(CurrentWeapon) && EnemyStatus == EEnemyStatus::Combat)
	{
		const FTransform WeaponSocketTransform = CurrentWeapon->GetMuzzleSocketTransform();		
		const FVector WeaponToPlayer = TargetViewLocation - WeaponSocketTransform.GetLocation();
		return WeaponToPlayer.Rotation();
	}
	return Super::GetViewRotation();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::SetDefaultColorType(const EColorType NewColorType)
{
	DefaultColorType = NewColorType;
	IBPE_InteractWithColorType::SetDefaultColorType(NewColorType);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Enemy::CanInvestigate() const
{
	return EnemyStatus != EEnemyStatus::Combat;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::OnColorTypeChanged(const EColorType ColorType)
{
	if(IsValid(GetMesh()) && BodyMaterialColor.Contains(ColorType))
	{
		UMaterialInstanceDynamic* EnemyMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0,GetMesh()->GetMaterial(0));
		if(IsValid(EnemyMaterial))
		{
			EnemyMaterial->SetVectorParameterValue("MainColor", FLinearColor(BodyMaterialColor[ColorType]));	
		}

		if(IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetDefaultColorType(ColorType);
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
	
	DropWeapon();
	
	SetLifeSpan(DestroyDelay);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Enemy::CanBeDamageableWithColor(const EColorType DamageColorType) const
{
	return DefaultColorType == DamageColorType || DamageColorType == EColorType::Multicolor;
}

//----------------------------------------------------------------------------------------------------------------------
float ABPE_Enemy::GetUltimateXP() const
{
	return XPValue;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Enemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPE_Enemy, DefaultColorType);
	DOREPLIFETIME(ABPE_Enemy, CurrentWeapon);
	DOREPLIFETIME(ABPE_Enemy, EnemyStatus);
}

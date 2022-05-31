// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BPE_Weapon.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ABPE_Weapon::ABPE_Weapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	
	PickupArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickupArea"));
	PickupArea->SetupAttachment(RootComponent);
	PickupArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	CurrentState = EWeaponState::Idle;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	/** check if we are on the server to enable PickupArea collision*/
	if(HasAuthority())
	{
		PickupArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupArea->OnComponentBeginOverlap.AddDynamic(this, &ABPE_Weapon::OnPlayerOverlap);
		PickupArea->OnComponentEndOverlap.AddDynamic(this, &ABPE_Weapon::OnPlayerEndOverlap);
	}

	SetWidgetVisibility(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABPE_PlayerCharacter* OverlappedCharacter = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter ))
	{
		OverlappedCharacter->SetOverlappingWeapon(this);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter ))
	{
		OverlappedCharacter->SetOverlappingWeapon(nullptr);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnRep_WeaponState()
{
	OnSetWeaponState();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnSetWeaponState()
{
	switch (CurrentState)
	{
	case EWeaponState::Idle:
		{
			const ECollisionEnabled::Type PickupAreaTypeCollision = HasAuthority()?
				ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
			
			SetWeaponParametersOnNewState(ECollisionEnabled::QueryAndPhysics, true,
				PickupAreaTypeCollision);
			break;
		}
	case EWeaponState::Equipped:
		{
			SetWeaponParametersOnNewState(ECollisionEnabled::NoCollision, false,
				ECollisionEnabled::NoCollision);
			break;
		}
	default:
		{
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetWeaponParametersOnNewState(ECollisionEnabled::Type MeshTypeCollision, bool bEnableMeshPhysics,
		ECollisionEnabled::Type PickupAreaTypeCollision)
{
	WeaponMesh->SetSimulatePhysics(bEnableMeshPhysics);
	WeaponMesh->SetEnableGravity(bEnableMeshPhysics);
	WeaponMesh->SetCollisionEnabled(MeshTypeCollision);
	
	PickupArea->SetCollisionEnabled(PickupAreaTypeCollision);

	SetWidgetVisibility(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetState(EWeaponState State)
{
	CurrentState = State;
	OnSetWeaponState();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetWidgetVisibility(bool bShowWidget)
{
	if(IsValid(PickupWidget))
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPE_Weapon, CurrentState);
}





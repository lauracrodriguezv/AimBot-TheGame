// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BPE_Weapon.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/BPE_Projectile.h"

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
	RoundsPerMinute = 600.0f;
	LastFireTime = 0.0f;
	ShotDistance = 10000.f;

	bCanFire = true;
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

	TimeBetweenShots = 60 / RoundsPerMinute;
}

void ABPE_Weapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
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
void ABPE_Weapon::Fire()
{
	if(IsValid(WeaponMesh) && IsValid(ProjectileBullet))
	{
		const USkeletalMeshSocket* MuzzleSocket = WeaponMesh->GetSocketByName(FName("SCK_MuzzleFlash"));
		if(IsValid(MuzzleSocket))
		{
			const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(WeaponMesh);

			TraceUnderCrosshairs(HitTarget);
			const FVector BulletDirection = HitTarget.ImpactPoint - SocketTransform.GetLocation();
			const FRotator BulletRotation = BulletDirection.Rotation();
			
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			SpawnParameters.Instigator = Cast<APawn>(GetOwner());
			
			GetWorld()->SpawnActor<ABPE_Projectile>(ProjectileBullet, SocketTransform.GetLocation(), BulletRotation, SpawnParameters);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::TraceUnderCrosshairs(FHitResult& OutTraceHitResult)
{
	FVector2D ViewportSize;
	if(IsValid(GEngine) && IsValid(GEngine->GameViewport))
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairLocation(ViewportSize.X / 2, ViewportSize.Y / 2);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		if(IsValid(OwnerCharacter))
		{
			const float DistanceToCharacter = (OwnerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.0f);
		}
		const FVector End = Start + CrosshairWorldDirection * ShotDistance;

		GetWorld()->LineTraceSingleByChannel(OutTraceHitResult, Start, End, ECC_Visibility);
		
		if(!OutTraceHitResult.bBlockingHit)
		{
			OutTraceHitResult.ImpactPoint = End;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::StopFire()
{
	if(bIsAutomatic)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::EnableFire()
{
	bCanFire = true;
	if(bIsAutomatic)
	{
		Fire();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::StartFire()
{
	if(bCanFire)
	{
		bCanFire = false;
		Fire();
		GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ABPE_Weapon::Fire, TimeBetweenShots, true, 0.15);		
	}
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





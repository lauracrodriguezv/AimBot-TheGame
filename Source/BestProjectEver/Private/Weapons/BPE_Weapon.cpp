// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BPE_Weapon.h"

#include "BestProjectEver/BestProjectEver.h"
#include "Character/BPE_Enemy.h"
#include "Character/BPE_PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ImageWriteQueue/Public/ImagePixelData.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/BPE_Casing.h"
#include "Weapons/BPE_Projectile.h"
#include "Sound/SoundCue.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_Weapon::ABPE_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
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
	ShotDistance = 10000.0f;
	ExtraDistance = 100.0f;

	MuzzleFlashSocketName = "SCK_MuzzleFlash";
	AmmoEjectSocketName = "SCK_AmmoEject";
	
	bCanFire = true;
	bIsAutomatic = false;

	ZoomedFOV = 30.0f;
	ZoomInterpSpeed = 20.0f;

	ShootType = EShootType::LineTrace;
	MinDistanceToImpactPoint = 300.0f;

	BaseDamage = 5.0f;

	ShootLoudness = 1.0f;

	ImpulseOnDropped = 1000.0f;

	DestroyDelay = 10.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateMeshColorType(DefaultColorType);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::InitializeReferences()
{
	/** check if we are on the server to enable PickupArea collision*/
	if(HasAuthority())
	{
		PickupArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupArea->OnComponentBeginOverlap.AddDynamic(this, &ABPE_Weapon::OnPickupAreaOverlap);
		PickupArea->OnComponentEndOverlap.AddDynamic(this, &ABPE_Weapon::OnPickupAreaEndOverlap);
	}

	SetWidgetVisibility(false);
	SetCurrentColorType(DefaultColorType);
	
	TimeBetweenShots = 60.0f / RoundsPerMinute;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnPickupAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABPE_PlayerCharacter* OverlappedCharacter = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter ))
	{
		OverlappedCharacter->SetOverlappingWeapon(this);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnPickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter))
	{
		OverlappedCharacter->SetOverlappingWeapon(nullptr);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::ShootWithLineTrace(const FVector& ImpactPoint)
{
	Multicast_PlayImpactFireEffects(ImpactPoint);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::ShootWithBullets(const FVector& ImpactPoint)
{
	if(IsValid(BulletClass))
	{
		const FTransform SocketTransform = GetMuzzleSocketTransform();
		const FVector BulletDirection = ImpactPoint - SocketTransform.GetLocation();
		const FRotator BulletRotation = BulletDirection.Rotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
			
		GetWorld()->SpawnActor<ABPE_Projectile>(BulletClass, SocketTransform.GetLocation(), BulletRotation, SpawnParameters);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::ShootWithLineTraceAndBullet(const FVector& TraceStart, const FVector& ImpactPoint)
{
	const float TraceLength = (ImpactPoint - TraceStart).Size();
	if(TraceLength < MinDistanceToImpactPoint)
	{
		ShootWithLineTrace(ImpactPoint);
	}
	else
	{
		ShootWithBullets(ImpactPoint);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetState(EWeaponState State)
{
	CurrentState = State;
	OnSetWeaponState();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::UpdateMeshColorType(const EColorType ColorType)
{
	if(IsValid(WeaponMesh) && MaterialColor.Contains(ColorType))
	{
		UMaterialInstanceDynamic* WeaponMaterial = WeaponMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0,WeaponMesh->GetMaterial(0));
		if(IsValid(WeaponMaterial))
		{
			WeaponMaterial->SetVectorParameterValue("ColorType", FLinearColor(MaterialColor[ColorType]));	
		}
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
			HandleIdleState();
			break;
		}
	case EWeaponState::Equipped:
		{
			HandleEquippedState();			
			break;
		}
	case EWeaponState::WaitingToDestroy:
		{
			HandleWaitingToDestroyState();			
			break;
		}
	default:
		{
			break;
		}
	}
	SetWidgetVisibility(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::HandleIdleState()
{
	SetPhysicsPropertiesToEquip();
	bIsAutomatic = true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::HandleEquippedState()
{
	UpdatePhysicsProperties(ECollisionEnabled::NoCollision, false,ECollisionEnabled::NoCollision);

	if(HasAuthority())
	{
		SetLifeSpan(0.0f);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::HandleWaitingToDestroyState()
{
	SetPhysicsPropertiesToEquip();
	bIsAutomatic = true;
	if(HasAuthority())
	{
		SetLifeSpan(DestroyDelay);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetPhysicsPropertiesToEquip()
{
	const ECollisionEnabled::Type PickupAreaTypeCollision = HasAuthority()? ECollisionEnabled::QueryAndPhysics :
		ECollisionEnabled::NoCollision;
	UpdatePhysicsProperties(ECollisionEnabled::QueryAndPhysics, true, PickupAreaTypeCollision);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::UpdatePhysicsProperties(ECollisionEnabled::Type MeshTypeCollision, bool bEnableMeshPhysics,
		ECollisionEnabled::Type PickupAreaTypeCollision)
{
	WeaponMesh->SetSimulatePhysics(bEnableMeshPhysics);
	WeaponMesh->SetEnableGravity(bEnableMeshPhysics);
	WeaponMesh->SetCollisionEnabled(MeshTypeCollision);
	
	PickupArea->SetCollisionEnabled(PickupAreaTypeCollision);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::Fire()
{
	FHitResult HitResult;

	TraceUnderCrosshairs(HitResult);
	const FVector MuzzleLocation = HitResult.TraceStart;
	Multicast_PlayMuzzleFireEffects(MuzzleLocation);
	
	if(IsValid(PlayerOwner))
	{
		PlayerOwner->CharacterMakeNoise(ShootLoudness, MuzzleLocation);
	}

	if(HitResult.bBlockingHit)
	{
		switch (ShootType)
		{
		case (EShootType::LineTrace):
			{
				ShootWithLineTrace(HitResult.ImpactPoint);
				break;
			}
		case (EShootType::Bullet):
			{
				ShootWithBullets(HitResult.ImpactPoint);
				break;
			}
		case (EShootType::Mixed):
			{
				ShootWithLineTraceAndBullet(HitResult.TraceStart, HitResult.ImpactPoint);
				break;
			}
		default:
			{
				break;
			}
		}
		
		ApplyDamageOnHit(HitResult);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::ApplyDamageOnHit(const FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if(IsValid(HitActor) && HitActor->Implements<UBPE_Damagable>())
	{
		const IBPE_Damagable* ActorDamageable = Cast<IBPE_Damagable>(HitActor);
		if(ActorDamageable->CanBeDamageableWithColor(CurrentColorType))
		{
			if(IsValid(PlayerOwner))
			{
				PlayerOwner->AddUltimateXP(ActorDamageable->GetUltimateXP());
			}
			
			const FVector ShootDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
			if(IsValid(OwnerCharacter))
			{
				UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShootDirection, HitResult,
					OwnerCharacter->GetInstigatorController(),OwnerCharacter, DamageType);		
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::ApplyDamage(const FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();
	
	if(IsValid(DamagedActor) && IsValid(OwnerCharacter))
	{
		const FVector ShootDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, ShootDirection, HitResult,
			OwnerCharacter->GetInstigatorController(),OwnerCharacter, DamageType);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::TraceUnderCrosshairs(FHitResult& OutHitResult)
{
	if(IsValid(OwnerCharacter))
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		
		OwnerCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		
		FVector TraceStart = EyeLocation;
		const float DistanceToPlayer = (OwnerCharacter->GetActorLocation() - TraceStart).Size();

		const FVector TraceDirection = EyeRotation.Vector();
		/** This additional distance is to prevent the shoot hit something behind the character */
		TraceStart += TraceDirection * (DistanceToPlayer + ExtraDistance);
		
		const FVector TraceEnd = EyeLocation + (TraceDirection * ShotDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECC_Weapon, QueryParams);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::StopFire()
{
	if(GetCurrentState() == EWeaponState::Firing)
	{
		SetState(EWeaponState::Equipped);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::StartFire()
{	
	if(bCanFire)
	{
		SetState(EWeaponState::Firing);
		bCanFire = false;
		Fire();
		GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ABPE_Weapon::HandleReFiring,
			TimeBetweenShots, true, TimeBetweenShots);		
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::HandleReFiring()
{
	bCanFire = true;
	if(bIsAutomatic && CurrentState == EWeaponState::Firing)
	{
		Fire();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	OnSetNewOwner();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	OnSetNewOwner();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnSetNewOwner()
{
	OwnerCharacter = Cast<ABPE_BaseCharacter>(Owner);
	PlayerOwner = Cast<ABPE_PlayerCharacter>(Owner);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnPickup(AActor* NewOwner)
{
	if(IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		SetState(EWeaponState::Equipped);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnDropped()
{
	if(IsHidden())
	{
		SetActorHiddenInGame(false);
	}
	
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRules);
	
	SetOwner(nullptr);
	SetState(EWeaponState::Idle);
	CurrentColorType = DefaultColorType;
	
	if(IsValid(WeaponMesh))
	{
		WeaponMesh->AddImpulse(FMath::VRand() * ImpulseOnDropped, NAME_None, true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::Multicast_PlayMuzzleFireEffects_Implementation(const FVector& MuzzleLocation)
{	
	if (IsValid(FireAnimation))
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	if(IsValid(ShootSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, MuzzleLocation, ShootLoudness);
	}
	
	if (IsValid(CasingClass))
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(AmmoEjectSocketName);
		if (IsValid(AmmoEjectSocket))
		{
			const FTransform AmmoSocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			GetWorld()->SpawnActor<ABPE_Casing>(CasingClass, AmmoSocketTransform.GetLocation(), AmmoSocketTransform.Rotator());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::Multicast_PlayImpactFireEffects_Implementation(const FVector& ImpactPoint)
{
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, ImpactPoint);
		//DrawDebugSphere(GetWorld(), ImpactPoint, 30.0f, 12.0f, FColor::Red, true, 1.0f, 0, 0.5f);

	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, ImpactPoint);
	}
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Weapon::Multicast_PlayImpactFireEffects_Validate(const FVector& ImpactPoint)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_Weapon::Multicast_PlayMuzzleFireEffects_Validate(const FVector& MuzzleLocation)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetCurrentColorType(const EColorType ColorType)
{	
	CurrentColorType = ColorType;
	IBPE_InteractWithColorType::SetCurrentColorType(ColorType);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::SetDefaultColorType(const EColorType ColorType)
{
	DefaultColorType = ColorType;
	IBPE_InteractWithColorType::SetDefaultColorType(ColorType);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnRep_ColorType()
{
	UpdateMeshColorType(CurrentColorType);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::OnStopInteraction()
{
	if(HasAuthority() && CurrentState != EWeaponState::Equipped)
	{
		SetState(EWeaponState::WaitingToDestroy);
		if(IsValid(WeaponMesh))
		{
			WeaponMesh->AddImpulse(FMath::VRand() * 1000.0f, NAME_None, true);
		}
	}
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
FTransform ABPE_Weapon::GetMuzzleSocketTransform() const
{
	if(IsValid(WeaponMesh))
	{
		const USkeletalMeshSocket* MuzzleSocket = WeaponMesh->GetSocketByName(MuzzleFlashSocketName);
		if(IsValid(MuzzleSocket))
		{
			return MuzzleSocket->GetSocketTransform(WeaponMesh);
		}
	}
	return FTransform();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPE_Weapon, CurrentState);
	DOREPLIFETIME(ABPE_Weapon, CurrentColorType);
	DOREPLIFETIME(ABPE_Weapon, DefaultColorType);
}





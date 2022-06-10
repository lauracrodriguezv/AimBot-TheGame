// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_BaseCharacter.h"

#include "BestProjectEver/BestProjectEver.h"
#include "Components/BPE_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_BaseCharacter::ABPE_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UBPE_HealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	NoiseEmitterComponent->SetAutoActivate(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Block);
	
	WeaponSocketName = "SCK_Weapon";
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetMesh()))
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ABPE_BaseCharacter::HandleCharacterDamage);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::HandleCharacterDeath()
{
	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::HandleCharacterDamage(UBPE_HealthComponent* CurrentHealthComponent, float CurrentHealth, float MaxHealth)
{
	if(HealthComponent->IsDead())
	{
		PlayMontage(DamageMontage, "Dead");
		HandleCharacterDeath();
	}
	else
	{
		PlayMontage(DamageMontage, "Hurt");
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StartWeaponFire()
{
	// empty in base class
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StopWeaponFire()
{
	// empty in base class
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::CharacterMakeNoise(const float Loudness, const FVector NoiseLocation)
{
	if(IsValid(NoiseEmitterComponent))
	{
		NoiseEmitterComponent->MakeNoise(this, Loudness, NoiseLocation);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::PlaySound(USoundCue* Sound)
{
	if(IsValid(Sound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName, float PlayRate /* = 1.0f */)
{
	if(IsValid(AnimInstance) && IsValid(Montage))
	{
		AnimInstance->Montage_Play(Montage, PlayRate);
		if(!SectionName.IsNone())
		{
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_BaseCharacter::IsWeaponEquipped() const 
{
	// true just in base class
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
			return CharacterA->Team == CharacterB->Team;
		}

		return true;
	}
	return true;
}
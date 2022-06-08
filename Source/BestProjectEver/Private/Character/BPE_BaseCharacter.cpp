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

	HealthComponent->OnDeathDelegate.AddDynamic(this, &ABPE_BaseCharacter::HandlePlayerDeath);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::HandlePlayerDeath()
{
	PlayAnimMontage(DeathMontage);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StartWeaponFire()
{
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::StopWeaponFire()
{
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
void ABPE_BaseCharacter::PlayMontage(UAnimMontage* Montage, float PlayRate)
{
	if(IsValid(AnimInstance) && IsValid(Montage))
	{
		AnimInstance->Montage_Play(Montage, PlayRate);
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
			return CharacterA->TeamNumber == CharacterB->TeamNumber;
		}

		return true;
	}
	return true;
}
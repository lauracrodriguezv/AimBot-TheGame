// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_BaseCharacter.h"

#include "BestProjectEver/BestProjectEver.h"
#include "Components/BPE_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Core/GameModes/BPE_LobbyGameMode.h"
#include "GameElements/BPE_HealingArea.h"
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
	
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ABPE_BaseCharacter::HandleCharacterHealthChange);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::HandleCharacterDeath(AActor* DamagedActor, AController* InstigatedBy, AActor* DamageCauser)
{
	if(IsValid( GetWorld()->GetAuthGameMode<ABPE_GameplayGameMode>()))
	{
		ABPE_GameplayGameMode* GameplayGameMode = GetWorld()->GetAuthGameMode<ABPE_GameplayGameMode>();
		GameplayGameMode->OnCharacterDeath(InstigatedBy, GetController(), this);
	}
	else if(IsValid(GetWorld()->GetAuthGameMode<ABPE_LobbyGameMode>()))
	{
		ABPE_LobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<ABPE_LobbyGameMode>();
		LobbyGameMode->OnCharacterDeath(InstigatedBy, GetController(), this);
	}

}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::HandleCharacterHealthChange(const FHealthData& HealthData)
{
	if(IsValid(HealthComponent) && HealthComponent->IsDead()) 
	{
		if(HasAuthority())
		{
			HandleCharacterDeath(HealthData.AffectedActor, HealthData.InstigatedBy, HealthData.HealthChangeCauser);	
		}
		
		PlayMontage(DamageMontage, "Dead");
		// play dissolve effect 
	}
	else
	{
		PlayMontage(DamageMontage, "Hurt");
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_BaseCharacter::DropWeapon()
{
	// empty in base class
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
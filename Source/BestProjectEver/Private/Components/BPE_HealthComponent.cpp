// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BPE_HealthComponent.h"

#include "Net/UnrealNetwork.h"


UBPE_HealthComponent::UBPE_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	bIsDead = false;
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	AActor* ActorOwner = GetOwner();
	if (IsValid(ActorOwner))
	{
		ActorOwner->OnTakeAnyDamage.AddDynamic(this, &UBPE_HealthComponent::HandleTakeAnyDamage);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	if (CurrentHealth == 0.0f) 
	{
		bIsDead = true;
	}

	OnHealthChange();
	UE_LOG(LogTemp, Log, TEXT("My Health Is: %f"), CurrentHealth);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_HealthComponent::OnRep_Health()
{
	UE_LOG(LogTemp, Log, TEXT("Client My Health Is: %f"), CurrentHealth);
	OnHealthChange();
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_HealthComponent::OnHealthChange()
{
	OnHealthChangeDelegate.Broadcast(this, CurrentHealth, MaxHealth);
}

//----------------------------------------------------------------------------------------------------------------------
void UBPE_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBPE_HealthComponent, CurrentHealth);
}


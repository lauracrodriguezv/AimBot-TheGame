// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/BPE_HealingArea.h"

#include "Character/BPE_BaseCharacter.h"
#include "Components/BPE_HealthComponent.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_HealingArea::ABPE_HealingArea()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMeshComponent"));
	SetRootComponent(AreaMeshComponent);
	AreaMeshComponent->SetCollisionResponseToChannels(ECR_Ignore);
	AreaMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealingAmount = 100.0f;
	CooldownTime = 10.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(AreaMeshComponent))
	{
		AreaMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABPE_HealingArea::OnAreaMeshOverlap);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::OnAreaMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ABPE_BaseCharacter* OverlappingCharacter = Cast<ABPE_BaseCharacter>(OtherActor);
	if(IsValid(OverlappingCharacter))
	{
		HealCharacter(OverlappingCharacter);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::HealCharacter(const ABPE_BaseCharacter* Character)
{
	if(HasAuthority() && IsValid(Character) && IsValid(Character->GetHealthComponent()))
	{
		Character->GetHealthComponent()->Heal(HealingAmount, this);
	}

	if(IsValid(AreaMeshComponent))
	{
		AreaMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AreaMeshComponent->SetVisibility(false);
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ABPE_HealingArea::RestoreHealingArea, CooldownTime);	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::RestoreHealingArea()
{
	if(IsValid(AreaMeshComponent))
	{
		AreaMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaMeshComponent->SetVisibility(true);
	}
}


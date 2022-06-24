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
		AreaMeshMaterialInstanceDynamic = AreaMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0,
			AreaMeshComponent->GetMaterial(0));
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::HealCharacter(const ABPE_BaseCharacter* Character)
{
	if(IsValid(Character->GetHealthComponent()))
	{
		Character->GetHealthComponent()->Heal(HealingAmount);
	}
	if(IsValid(AreaMeshComponent))
	{
		AreaMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(IsValid(AreaMeshMaterialInstanceDynamic))
		{
			AreaMeshMaterialInstanceDynamic->SetScalarParameterValue("Opacity", 0.0f);	
		}
	}
	GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ABPE_HealingArea::RestoreHealingArea, CooldownTime);	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_HealingArea::RestoreHealingArea()
{
	if(IsValid(AreaMeshComponent))
	{
		AreaMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		if(IsValid(AreaMeshMaterialInstanceDynamic))
		{
			AreaMeshMaterialInstanceDynamic->SetScalarParameterValue("Opacity", 0.1f);	
		}
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


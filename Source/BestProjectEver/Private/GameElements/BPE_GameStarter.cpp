// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/BPE_GameStarter.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/GameModes/BPE_LobbyGameMode.h"
#include "Core/GameState/BPE_GameState.h"
#include "Net/UnrealNetwork.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_GameStarter::ABPE_GameStarter()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalPlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalPlaneMesh"));
	PortalPlaneMesh->SetupAttachment(RootComponent);

	ActivationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationTrigger"));
	ActivationTrigger->SetupAttachment(PortalPlaneMesh);
	ActivationTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InformationWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InformationWidget"));
	InformationWidget->SetupAttachment(RootComponent);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::InitializeReferences()
{
	if(IsValid(ActivationTrigger))
	{
		ActivationTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABPE_GameStarter::OnActivationTriggerOverlap);
	}

	if(IsValid(InformationWidget))
	{
		InformationWidget->SetVisibility(false);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	TravelToGameplayMap();
	
	if(IsValid(InformationWidget))
	{
		InformationWidget->SetVisibility(true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::TravelToGameplayMap()
{
	if(HasAuthority())
	{
		ABPE_LobbyGameMode* LobbyGameMode = Cast<ABPE_LobbyGameMode>(GetWorld()->GetAuthGameMode());
		if(IsValid(LobbyGameMode))
		{
			LobbyGameMode->TravelToMap(FString("/Game/Maps/GameplayMap?listen"));	
		}
	}
	else
	{
		Server_TravelToGameplayMap();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::Server_TravelToGameplayMap_Implementation()
{
	TravelToGameplayMap();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_GameStarter::Server_TravelToGameplayMap_Validate()
{
	return true;
}

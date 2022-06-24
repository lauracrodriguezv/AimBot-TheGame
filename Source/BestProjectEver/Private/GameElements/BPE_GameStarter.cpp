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

	PortalFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalFrameMesh"));
	SetRootComponent(PortalFrameMesh);

	PortalPlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalPlaneMesh"));
	PortalPlaneMesh->SetupAttachment(RootComponent);

	ActivationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationTrigger"));
	ActivationTrigger->SetupAttachment(PortalPlaneMesh);
	ActivationTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);

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
	bTravelSucceeded = false;
	if(HasAuthority() && IsValid(ActivationTrigger))
	{
		ActivationTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		ActivationTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABPE_GameStarter::OnActivationTriggerOverlap);
	}

	if(IsValid(InformationWidget))
	{
		InformationWidget->SetVisibility(false);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::OnRep_TravelSucceed()
{
	if(IsValid(InformationWidget))
	{
		InformationWidget->SetVisibility(bTravelSucceeded);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->ServerTravel(FString("/Game/Maps/GameplayMap?listen"));
	if(IsValid(InformationWidget))
	{
		InformationWidget->SetVisibility(bTravelSucceeded);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_GameStarter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABPE_GameStarter, bTravelSucceeded);
}

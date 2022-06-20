


#include "GameElements/BPE_SpawnPad.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "Interfaces/BPE_InteractWithColorType.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_SpawnPad::ABPE_SpawnPad()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	SpawnPadBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPadBaseMesh"));
	SetRootComponent(SpawnPadBaseMesh);

	SpawnPadRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPadRingMesh"));
	SpawnPadRingMesh->SetupAttachment(RootComponent);
	SpawnPadRingMesh->SetIsReplicated(true);
	
	HoloGridPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoloGridPlane"));
	HoloGridPlane->SetupAttachment(SpawnPadRingMesh);

	SpawnTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLocation"));
	SpawnTransform->SetupAttachment(SpawnPadBaseMesh);
	SpawnTransform->SetRelativeLocation(FVector(0.0f, 0.0f, 95.0f));

	FrameButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameButtonMesh"));
	FrameButtonMesh->SetupAttachment(RootComponent);
	
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(FrameButtonMesh);
	
	ActivationTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationTrigger"));
	ActivationTrigger->SetupAttachment(FrameButtonMesh);
	ActivationTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationTrigger->SetSphereRadius(130.0f);

	FX_SpawnPadActive = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX_SpawnPadActive"));
	FX_SpawnPadActive->SetupAttachment(RootComponent);
	FX_SpawnPadActive->SetRelativeLocation(FVector(0.0f, 0.0f, 27.0f));

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(RootComponent);

	RingMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RingMovementTimeline"));

	NewLocationMultiplier = FVector(0.0f, 0.0f, 200.0f);
	SetButtonEmissiveDelay = 2.0f;
	ButtonEmissive = 6.0f;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::InitializeReferences()
{
	if(HasAuthority() && IsValid(ActivationTrigger))
	{
		ActivationTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		ActivationTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABPE_SpawnPad::OnActivationTriggerOverlap);
		ActivationTrigger->OnComponentEndOverlap.AddDynamic(this, &ABPE_SpawnPad::OnActivationTriggerEndOverlap);
	}

	SetWidgetVisibility(false);

	if(IsValid(RingMovementCurve) && IsValid(RingMovementTimeline))
	{
		RingMovementTrack.BindDynamic(this, &ABPE_SpawnPad::UpdateRingPosition);
		RingMovementTimeline->AddInterpFloat(RingMovementCurve, RingMovementTrack);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter))
	{
		OverlappedCharacter->SetOverlappingSpawnPad(this);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter))
	{
		OverlappedCharacter->SetOverlappingSpawnPad(nullptr);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::SetWidgetVisibility(bool bShowWidget)
{
	if(IsValid(InteractWidget))
	{
		InteractWidget->SetVisibility(bShowWidget);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::SpawnActor()
{
	if(!HasAuthority())
	{
		return;
	}
	
	if(IsValid(CurrentActorSpawned))
	{
		if(CurrentActorSpawned->Implements<UBPE_InteractWithColorType>())
		{
			IBPE_InteractWithColorType* ActorWithColorType = Cast<IBPE_InteractWithColorType>(CurrentActorSpawned);
			ActorWithColorType->OnStopInteraction();
		}
		else
		{
			CurrentActorSpawned->Destroy();
		}
	}

	if(IsValid(ActorToSpawnClass))
	{
		const FTransform ActorTransform = SpawnTransform->GetComponentTransform();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		
		CurrentActorSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawnClass, ActorTransform.GetLocation(),
			ActorTransform.GetRotation().Rotator(), SpawnParameters);
		
		if(IsValid(CurrentActorSpawned) && CurrentActorSpawned->Implements<UBPE_InteractWithColorType>())
		{
			IBPE_InteractWithColorType* ActorWithColorType = Cast<IBPE_InteractWithColorType>(CurrentActorSpawned);
			/** this range was set taking into account that there are just three colors in the enum */
			const uint8 RandomColor = FMath::RandRange(0, 2);
			ActorWithColorType->SetColorType(static_cast<EColorType>(RandomColor));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::ActiveEffectsOnSpawn()
{
	if(IsValid(RingMovementTimeline))
	{
		RingMovementTimeline->PlayFromStart();
	}

	Multicast_SetButtonMeshMaterial();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::Multicast_SetButtonMeshMaterial_Implementation()
{
	if(IsValid(ButtonMesh))
	{
		ButtonMaterial = ButtonMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0,ButtonMesh->GetMaterial(0));
		if(IsValid(ButtonMaterial))
		{
			ButtonMaterial->SetScalarParameterValue("Emissive", ButtonEmissive);
			GetWorldTimerManager().SetTimer(TimerHandle_ButtonEmissive, this, &ABPE_SpawnPad::TurnOffButtonEmissive,
				SetButtonEmissiveDelay, false, SetButtonEmissiveDelay);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::TurnOffButtonEmissive()
{
	if(IsValid(ButtonMaterial))
	{
		ButtonMaterial->SetScalarParameterValue("Emissive", 0.0f);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::UpdateRingPosition(float Value)
{
	if(IsValid(SpawnPadRingMesh))
	{
		SpawnPadRingMesh->SetRelativeLocation(NewLocationMultiplier * Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnInteract()
{
	SpawnActor();
	ActiveEffectsOnSpawn();
}
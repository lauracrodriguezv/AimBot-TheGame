


#include "GameElements/BPE_SpawnPad.h"

#include "Character/BPE_PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_SpawnPad::ABPE_SpawnPad()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnPadBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPadBaseMesh"));
	SetRootComponent(SpawnPadBaseMesh);

	SpawnPadRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPadRingMesh"));
	SpawnPadRingMesh->SetupAttachment(RootComponent);
	
	HoloGridPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoloGridPlane"));
	HoloGridPlane->SetupAttachment(SpawnPadRingMesh);

	SpawnTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLocation"));
	SpawnTransform->SetupAttachment(SpawnPadBaseMesh);
	SpawnTransform->SetRelativeLocation(FVector(0.0f, 0.0f, 95.0f));

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);
	
	ActivationTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationTrigger"));
	ActivationTrigger->SetupAttachment(ButtonMesh);
	ActivationTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationTrigger->SetSphereRadius(130.0f);

	FX_SpawnPadActive = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX_SpawnPadActive"));
	FX_SpawnPadActive->SetupAttachment(RootComponent);
	FX_SpawnPadActive->SetRelativeLocation(FVector(0.0f, 0.0f, 27.0f));

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(ButtonMesh);
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
	if(HasAuthority())
	{
		ActivationTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		ActivationTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABPE_SpawnPad::OnActivationTriggerOverlap);
		ActivationTrigger->OnComponentEndOverlap.AddDynamic(this, &ABPE_SpawnPad::OnActivationTriggerEndOverlap);
	}

	SetWidgetVisibility(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter))
	{
		// set interactive trigger 
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABPE_PlayerCharacter* OverlappedCharacter  = Cast<ABPE_PlayerCharacter>(OtherActor);
	if(IsValid(OverlappedCharacter))
	{
		// set interactive trigger to nullptr
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
		// use an interface to decided what happen when now is not the CurrentActorSpawned
		// Mesh->AddImpulse(FMath::VRand() * 1000.0f, NAME_None, true);
	}

	if(IsValid(ActorToSpawnClass))
	{
		const FTransform ActorTransform = SpawnTransform->GetComponentTransform();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		
		CurrentActorSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawnClass, ActorTransform.GetLocation(),
			ActorTransform.GetRotation().Rotator(), SpawnParameters);
		
		// can be spawned with color?
			// set random color and update mesh color in enemy and weapon
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::ActiveEffectsOnSpawn()
{
	RingMovementTrack.BindDynamic(this, &ABPE_SpawnPad::UpdateRingPosition);
	if(IsValid(RingMovementCurve) && IsValid(RinMovementTimeline))
	{
		RinMovementTimeline->AddInterpFloat(RingMovementCurve, RingMovementTrack);
		RinMovementTimeline->Play();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::UpdateRingPosition(float DissolveValue)
{
	if(IsValid(SpawnPadRingMesh))
	{
		const FVector NewLocationMultiplier (0.0f, 0.0f, 200.0f);
		SpawnPadRingMesh->SetRelativeLocation(NewLocationMultiplier * DissolveValue);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_SpawnPad::OnInteract()
{
	SpawnActor();
	ActiveEffectsOnSpawn();
}
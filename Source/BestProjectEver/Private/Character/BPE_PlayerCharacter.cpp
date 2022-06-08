// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/BPE_HealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/BPE_Weapon.h"

//----------------------------------------------------------------------------------------------------------------------
ABPE_PlayerCharacter::ABPE_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
	bIsLookInverted = true;

	bIsAiming = false;
	DefaultWalkSpeed = 600.0f;
	AimWalkSpeed = 100.0f;
	ZoomOutInterpSpeed =  20.0f;

	TeamNumber = 1;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeReference();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::InitializeReference()
{
	DefaultFOV = CameraComponent->FieldOfView;
	CurrentFOV = DefaultFOV;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveForward(float Value)
{
	if(IsValid(Controller)  && Value != 0.0f)
	{
		const FRotator YawRotation (0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveRight(float Value)
{
	if(IsValid(Controller) && Value != 0.0f)
	{
		const FRotator YawRotation (0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::TurnAtRate(float Value)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::LookUpAtRate(float Value)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::AddControllerPitchInput(float Value)
{
	Super::AddControllerPitchInput(bIsLookInverted ? -Value : Value);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StartCrouch()
{
	Crouch();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EndCrouch()
{
	UnCrouch();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StartWeaponFire()
{
	if(IsValid(CurrentWeapon))
	{
		if (HasAuthority())
		{
			OnStartFire();
		}
		else
		{
			Server_StartFire();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StopWeaponFire()
{
	if(IsValid(CurrentWeapon))
	{
		if(HasAuthority())
		{
			OnStopFire();
		}
		else
		{
			Server_StopFire();
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_StartFire_Implementation()
{
	OnStartFire();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_StartFire_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnStartFire()
{
	CurrentWeapon->StartFire();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_StopFire_Implementation()
{
	OnStopFire();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_StopFire_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnStopFire()
{
	CurrentWeapon->StopFire();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Aim()
{
	if(IsValid(CurrentWeapon))
	{
		if(HasAuthority())
		{
			bIsAiming = !bIsAiming;
			OnIsAimingChanged();
		}
		else
		{
			Server_SetAiming();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_SetAiming_Implementation()
{
	Aim();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_SetAiming_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnIsAimingChanged()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming? AimWalkSpeed : DefaultWalkSpeed;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_Aiming()
{
	OnIsAimingChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipWeapon()
{
	if(IsValid(OverlappingWeapon))
	{
		if(HasAuthority())
		{
			HandleEquipWeapon(OverlappingWeapon);
		}
		else
		{
			Server_EquipWeapon(OverlappingWeapon);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipNextWeapon()
{
	if(Inventory.Num() >= 2)
	{
		/** Index of by key requieres more performance than store an index, but I did it this way being concsious that in 
		 * the inventory there is a maximun of 3 elements
		 */
		const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
		const int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Inventory.Num();
		ABPE_Weapon* NextWeapon = Inventory[NextWeaponIndex];
		Server_SetAsCurrentWeapon(NextWeapon);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipPreviousWeapon()
{
	if(Inventory.Num() >= 2)
	{
		const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
		const int32 NextWeaponIndex = (CurrentWeaponIndex - 1 + Inventory.Num()) % Inventory.Num();
		ABPE_Weapon* PreviousWeapon = Inventory[NextWeaponIndex];
		Server_SetAsCurrentWeapon(PreviousWeapon);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_EquipWeapon_Implementation(ABPE_Weapon* WeaponToEquip)
{
	HandleEquipWeapon(WeaponToEquip);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_EquipWeapon_Validate(ABPE_Weapon* WeaponToEquip)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::HandleEquipWeapon(ABPE_Weapon* WeaponToEquip)
{
	bool bIsColorInInventory = IsValid(FindWeaponWithColorType(WeaponToEquip->GetColorType()));
	if(bIsColorInInventory)
	{
		/*Check the ammo to reload current weapon or add to inventory this new weapon if it is more powerful*/
	}
	else
	{
		PickupWeapon(WeaponToEquip);
		SetAsCurrentWeapon(WeaponToEquip);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::PickupWeapon(ABPE_Weapon* NewWeapon)
{
	if(IsValid(NewWeapon) && HasAuthority())
	{
		NewWeapon->OnPickup(this);
		Inventory.AddUnique(NewWeapon);
		OnInventoryChanged();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_SetAsCurrentWeapon_Implementation(ABPE_Weapon* Weapon)
{
	SetAsCurrentWeapon(Weapon);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_SetAsCurrentWeapon_Validate(ABPE_Weapon* Weapon)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetAsCurrentWeapon(ABPE_Weapon* Weapon)
{
	if(IsValid(CurrentWeapon))
	{
		HideUnusedWeapon(CurrentWeapon);
	}
	
	if(IsValid(Weapon))
	{
		CurrentWeapon = Weapon;
		if(CurrentWeapon->IsHidden())
		{
			CurrentWeapon->SetActorHiddenInGame(false);
		}
		/** Attach the weapon on the socket is replicated to the clients */
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		OnCurrentWeaponChanged();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::HideUnusedWeapon(ABPE_Weapon* Weapon)
{
	if(IsValid(Weapon))
	{
		const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		Weapon->DetachFromActor(DetachRules);
		Weapon->SetActorHiddenInGame(true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_Inventory()
{
	OnInventoryChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnInventoryChanged()
{
	PlaySound(PickupSound);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetOverlappingWeapon(ABPE_Weapon* Weapon)
{
	ABPE_Weapon* LastOverlappingWeapon = OverlappingWeapon;
	OverlappingWeapon = Weapon;

	OnSetOverlappingWeapon(LastOverlappingWeapon);	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_CurrentWeapon()
{
	OnCurrentWeaponChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnCurrentWeaponChanged()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	PlayMontage(SwapWeaponMontage, 2.0f);

	if(IsValid(CurrentWeapon) && IsLocallyControlled())
	{
		OnChangeCurrentWeaponDelegate.Broadcast(CurrentWeapon->GetColorType());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_OverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon)
{
	OnSetOverlappingWeapon(LastOverlappingWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnSetOverlappingWeapon(ABPE_Weapon* LastOverlappingWeapon)
{
	if(IsLocallyControlled())
	{
		if(IsValid(OverlappingWeapon))
		{
			OverlappingWeapon->SetWidgetVisibility(true);
		}
		if(IsValid(LastOverlappingWeapon))
		{
			LastOverlappingWeapon->SetWidgetVisibility(false);
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
ABPE_Weapon* ABPE_PlayerCharacter::FindWeaponWithColorType(EColorType ColorType) const 
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i]->GetColorType() == ColorType)
		{
			return Inventory[i];
		}
	}
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::InterpolateFieldOfView(float DeltaSeconds)
{
	if (bIsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, CurrentWeapon->GetZoomedFOV(), DeltaSeconds, CurrentWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaSeconds, ZoomOutInterpSpeed);
	}
	if(IsLocallyControlled())
	{
		CameraComponent->SetFieldOfView(CurrentFOV);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(IsValid(CurrentWeapon))
	{
		InterpolateFieldOfView(DeltaSeconds);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABPE_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABPE_PlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABPE_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ABPE_PlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABPE_PlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABPE_PlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this,  &ABPE_PlayerCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABPE_PlayerCharacter::StartWeaponFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this,  &ABPE_PlayerCharacter::StopWeaponFire);

	PlayerInputComponent->BindAction("Equip",IE_Pressed, this, &ABPE_PlayerCharacter::EquipWeapon);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABPE_PlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABPE_PlayerCharacter::Aim);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABPE_PlayerCharacter::EquipNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this,  &ABPE_PlayerCharacter::EquipPreviousWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentWeapon);
	DOREPLIFETIME_CONDITION_NOTIFY(ABPE_PlayerCharacter, bIsAiming, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, Inventory, COND_OwnerOnly);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::IsWeaponEquipped() const 
{
	return IsValid(CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
FVector ABPE_PlayerCharacter::GetPawnViewLocation() const
{
	if(IsValid(CameraComponent))
	{
		return CameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}


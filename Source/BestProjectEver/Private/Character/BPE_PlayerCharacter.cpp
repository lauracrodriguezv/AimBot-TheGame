// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	CurrentWeapon->SetState(EWeaponState::Firing);
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
	if(CurrentWeapon->GetCurrentState() == EWeaponState::Firing)
	{
		CurrentWeapon->SetState(EWeaponState::Equipped);	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Aim()
{
	if(IsValid(CurrentWeapon))
	{
		bIsAiming = !bIsAiming;
		if(HasAuthority())
		{
			SetAiming();
		}
		else
		{
			Server_SetAiming(bIsAiming);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_SetAiming_Implementation(bool bIsPlayerAiming)
{
	bIsAiming = bIsPlayerAiming;
	SetAiming();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_SetAiming_Validate(bool bIsPlayerAiming)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetAiming()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming? AimWalkSpeed : DefaultWalkSpeed;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipWeapon()
{
	if(IsValid(OverlappingWeapon))
	{
		if(HasAuthority())
		{
			SetEquippedWeapon(OverlappingWeapon, CurrentWeapon);
		}
		else
		{
			Server_EquipWeapon(OverlappingWeapon);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_EquipWeapon_Implementation(ABPE_Weapon* WeaponToEquip)
{
	SetEquippedWeapon(WeaponToEquip, CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_EquipWeapon_Validate(ABPE_Weapon* WeaponToEquip)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetEquippedWeapon(ABPE_Weapon* WeaponToEquip, ABPE_Weapon* LastWeapon)
{
	if(IsValid(LastWeapon))
	{
		LastWeapon->SetState(EWeaponState::Idle);
		
		const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		LastWeapon->DetachFromActor(DetachRules);
		LastWeapon->SetOwner(nullptr);
	}
	
	if(IsValid(WeaponToEquip))
	{
		CurrentWeapon = WeaponToEquip;
		CurrentWeapon->SetState(EWeaponState::Equipped);
		
		/** Attach the weapon on the socket is replicated to the clients */
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		CurrentWeapon->SetOwner(this);
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
	
	CameraComponent->SetFieldOfView(CurrentFOV);
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
void ABPE_PlayerCharacter::SetOverlappingWeapon(ABPE_Weapon* Weapon)
{
	const TObjectPtr<ABPE_Weapon> LastOverlappingWeapon = OverlappingWeapon;
	OverlappingWeapon = Weapon;

	OnSetOverlappingWeapon(LastOverlappingWeapon);	
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

}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::IsEquipped() const 
{
	return IsValid(CurrentWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
FTransform ABPE_PlayerCharacter::GetCameraTransform() const
{
	return CameraComponent->GetComponentTransform();
}

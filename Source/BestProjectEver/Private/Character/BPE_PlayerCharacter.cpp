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
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bIsLookInverted = true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation (0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.0f)
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
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
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
void ABPE_PlayerCharacter::SetOverlappingWeapon(ABPE_Weapon* Weapon)
{
	ABPE_Weapon* LastOverlappingWeapon = OverlappingWeapon;
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

	PlayerInputComponent->BindAction("Equip",IE_Pressed, this, &ABPE_PlayerCharacter::EquipWeapon);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentWeapon);
}

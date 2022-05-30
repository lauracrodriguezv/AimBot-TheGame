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
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bIsLookInversion = true;
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
	Super::AddControllerPitchInput(bIsLookInversion ? -Value : Value);
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
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
}

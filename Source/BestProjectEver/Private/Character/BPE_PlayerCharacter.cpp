// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BPE_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/BPE_HealthComponent.h"
#include "Core/GameModes/BPE_GameplayGameMode.h"
#include "Core/GameState/BPE_GameState.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/BPE_Weapon.h"
#include "GameElements/BPE_SpawnPad.h"
#include "PlayerController/BPE_PlayerController.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceConstant.h"

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

	Team = ETeam::Player;

	bIsUsingUltimate = false;
	bCanUseUltimate = false;
	MaxUltimateXP = 100.0f;
	CurrentUltimateXP = 0.0f;
	MaxUltimateDuration = 10.0f;
	CurrentUltimateDuration = 0.0f;
	UltimateRate = 0.01f;
	UltimateWalkSpeed = 1'500.0f;
	UltimatePlayRate = 2.0f;
	XPValue = -10.0f;

	PlayRate = 1.0f;
	
	CarriedAmmo = 60;
	MaxCarriedAmmo = 300;
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

	GameStateReference = Cast<ABPE_GameState>(GetWorld()->GetGameState());
	if(IsValid(GameStateReference))
	{
		GameStateReference->OnMatchStateSet.AddDynamic(this, &ABPE_PlayerCharacter::OnMatchStateChange);
	}

	if(IsValid(GetMesh()))
	{
		DefaultPlayerMaterial = GetMesh()->GetMaterial(0);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PlayerControllerReference = Cast<ABPE_PlayerController>(NewController);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	PlayerControllerReference = Cast<ABPE_PlayerController>(GetController());
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnMatchStateChange(const FName MatchState)
{
	if(MatchState == MatchState::Cooldown)
	{
		StopWeaponFire();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::HandleCharacterDeath(AActor* KilledActor, AController* InstigatedBy, AActor* Killer)
{
	Super::HandleCharacterDeath(KilledActor, InstigatedBy, Killer);
	
	if(!Inventory.IsEmpty())
	{
		DropWeapon();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::DropWeapon()
{
	if(HasAuthority())
	{
		for (ABPE_Weapon* Weapon : Inventory)
		{
			if(IsValid(Weapon))
			{
				Weapon->OnDropped();
			}
		}
		
		Inventory.Empty();
		CurrentWeapon = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveForward(float Value)
{
	if(IsValid(Controller)  && Value != 0.0f && AreGameplayInputsEnabled())
	{
		const FRotator YawRotation (0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::MoveRight(float Value)
{
	if(IsValid(Controller) && Value != 0.0f && AreGameplayInputsEnabled())
	{
		const FRotator YawRotation (0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::TurnAtRate(float Value)
{
	if(IsValid(GameStateReference) && !GameStateReference->IsGamePaused())
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::LookUpAtRate(float Value)
{
	if(IsValid(GameStateReference) && !GameStateReference->IsGamePaused())
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::AddControllerPitchInput(float Value)
{
	if(IsValid(GameStateReference) && !GameStateReference->IsGamePaused())
	{
		Super::AddControllerPitchInput(bIsLookInverted ? -Value : Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::AddControllerYawInput(float Val)
{
	if(IsValid(GameStateReference) && !GameStateReference->IsGamePaused())
	{
		Super::AddControllerYawInput(Val);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Jump()
{
	if(AreGameplayInputsEnabled())
	{
		Super::Jump();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StopJumping()
{
	if(AreGameplayInputsEnabled())
	{
		Super::StopJumping();	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StartCrouch()
{
	if(AreGameplayInputsEnabled())
	{
		Crouch();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EndCrouch()
{
	if(AreGameplayInputsEnabled())
	{
		UnCrouch();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StartWeaponFire()
{
	if(IsValid(CurrentWeapon) && AreGameplayInputsEnabled())
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
	if(IsValid(CurrentWeapon) && AreGameplayInputsEnabled())
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
	if(!bIsUsingUltimate)
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : DefaultWalkSpeed;
	}
	
	if(IsLocallyControlled())
	{
		UMaterialInterface* PlayerMaterial = bIsAiming ? PlayerAimMaterialInstanceConstant : bIsUsingUltimate? RainbowMaterialInstanceConstant : DefaultPlayerMaterial;
		if(GetMesh())
		{
			GetMesh()->SetMaterial(0, PlayerMaterial);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_Aiming()
{
	OnIsAimingChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipWeapon()
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

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::EquipNextWeapon()
{
	if(Inventory.Num() >= 2 && AreGameplayInputsEnabled())
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
	if(Inventory.Num() >= 2 && AreGameplayInputsEnabled())
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
	bool bIsColorInInventory = IsValid(FindWeaponWithColorType(WeaponToEquip->GetDefaultColorType()));
	if(bIsColorInInventory)
	{
		TakeWeaponAmmo(WeaponToEquip);
	}
	else
	{
		PickupWeapon(WeaponToEquip);
		SetAsCurrentWeapon(WeaponToEquip);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Interact()
{
	if(AreGameplayInputsEnabled())
	{
		if(IsValid(OverlappingWeapon))
		{
			EquipWeapon();
		}
	
		if(IsValid(OverlappingSpawnPad))
		{
			ActivateSpawnPad();
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::ActivateSpawnPad()
{
	if(HasAuthority())
	{
		OnSpawnPadActivated();
	}
	else
	{
		Server_ActivateSpawnPad();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::StartUltimate()
{
	if(HasAuthority())
	{
		HandleUltimateStart();
	}
	else
	{
		Server_StartUltimate();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_StartUltimate_Implementation()
{
	StartUltimate();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_StartUltimate_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::HandleUltimateStart()
{
	if(bCanUseUltimate && !bIsUsingUltimate)
	{
		bIsUsingUltimate = true;
		OnIsUsingUltimateChanged();
		
		bCanUseUltimate = false;
			
		CurrentUltimateDuration = MaxUltimateDuration;
		const FTimerDelegate UltimateTimerDelegate = FTimerDelegate::CreateUObject(this, &ABPE_PlayerCharacter::UpdateUltimateDuration, UltimateRate);
		GetWorldTimerManager().SetTimer(TimerHandle_Ultimate, UltimateTimerDelegate, UltimateRate, true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::UpdateUltimateDuration(const float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	OnUltimateValueUpdated();
	
	if(FMath::IsNearlyZero(CurrentUltimateDuration))
	{
		CurrentUltimateXP = 0.0f;
		bIsUsingUltimate = false;
		OnIsUsingUltimateChanged();
		
		GetWorldTimerManager().ClearTimer(TimerHandle_Ultimate);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_UsingUltimate()
{
	OnIsUsingUltimateChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_UltimateValue()
{
	OnUltimateValueUpdated();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnUltimateValueUpdated()
{
	if(bIsUsingUltimate)
	{
		OnUltimateUpdate.Broadcast(CurrentUltimateDuration * 10.0f, MaxUltimateDuration * 10.0f);
	}
	else
	{
		OnUltimateUpdate.Broadcast(CurrentUltimateXP, MaxUltimateXP);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnIsUsingUltimateChanged()
{
	OnChangeUltimateStatus.Broadcast(bIsUsingUltimate);
	
	GetCharacterMovement()->MaxWalkSpeed = bIsUsingUltimate? UltimateWalkSpeed : DefaultWalkSpeed;
	PlayRate = bIsUsingUltimate? UltimateRate : 1.0f;
	
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IsValid(Inventory[i]))
		{
			const EColorType WeaponColorType = bIsUsingUltimate? EColorType::Multicolor : Inventory[i]->GetDefaultColorType();
			Inventory[i]->SetCurrentColorType(WeaponColorType);
		}
	}
	
	PlayUltimateEffects();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::PlayUltimateEffects()
{
	UMaterialInterface* PlayerMaterial = bIsUsingUltimate ? RainbowMaterialInstanceConstant : DefaultPlayerMaterial;
	if(GetMesh())
	{
		GetMesh()->SetMaterial(0, PlayerMaterial);
	}
	
	if(bIsUsingUltimate)
	{
		if(IsValid(UltimateEffect))
		{
			const FVector UltimateEffectSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.0f);
			UltimateParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateEffect, UltimateEffectSpawnPoint, GetActorRotation());
		}

		if(IsValid(UltimateSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), UltimateSound, GetActorLocation());
		}	
	}
	else
	{
		if(IsValid(UltimateParticleComponent))
		{
			UltimateParticleComponent->DestroyComponent();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Reload()
{
	if(AreGameplayInputsEnabled() && IsValid(CurrentWeapon) && CarriedAmmo > 0)
	{
		if(HasAuthority())
		{
			HandleReloading();
		}
		else
		{
			Server_Reload();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_Reload_Implementation()
{
	Reload();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_Reload_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::HandleReloading()
{
	if(IsValid(CurrentWeapon) && !CurrentWeapon->IsMagFull())
	{
		const int32 AvailableSpaceInMag = CurrentWeapon->GetMagCapacity() - CurrentWeapon->GetCurrentAmmo();
		const int32 AmmoToReload = FMath::Min(AvailableSpaceInMag, CarriedAmmo);
		
		CarriedAmmo = FMath::Clamp(CarriedAmmo - AmmoToReload, 0, MaxCarriedAmmo);
		OnCarriedAmmoUpdate();

		CurrentWeapon->OnReloading(AmmoToReload);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::TakeWeaponAmmo(ABPE_Weapon* Weapon)
{
	if(IsValid(Weapon) && !Weapon->IsEmpty())
	{
		const int32 AvailableSpaceCarriedAmmo = MaxCarriedAmmo - CarriedAmmo;
		const int32 AmmoToReload = FMath::Min(AvailableSpaceCarriedAmmo, Weapon->GetCurrentAmmo());
		
		CarriedAmmo = FMath::Clamp(CarriedAmmo + AmmoToReload, 0, MaxCarriedAmmo);
		OnCarriedAmmoUpdate();
		
		Weapon->OnReloading(-AmmoToReload);
	}
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnRep_CarriedAmmo()
{
	OnCarriedAmmoUpdate();
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnCarriedAmmoUpdate()
{
	if(IsLocallyControlled())
	{
		OnCarriedAmmoChanged.Broadcast(CarriedAmmo);
	}

	if(IsValid(ReloadMontage) && IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(ReloadMontage);
	}
}


//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::Server_ActivateSpawnPad_Implementation()
{
	OnSpawnPadActivated();
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::Server_ActivateSpawnPad_Validate()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnSpawnPadActivated()
{
	if(IsValid(OverlappingSpawnPad))
	{
		OverlappingSpawnPad->OnInteract();
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
void ABPE_PlayerCharacter::PauseGame()
{
	if(IsValid(PlayerControllerReference) && AreGameplayInputsEnabled())
	{
		PlayerControllerReference->SetGamePause(true);
	}
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::CanBeDamageableWithColor(const EColorType DamageColorType) const
{
	return !bIsUsingUltimate;
}

//----------------------------------------------------------------------------------------------------------------------
float ABPE_PlayerCharacter::GetUltimateXP() const
{
	return XPValue;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetOverlappingWeapon(ABPE_Weapon* Weapon)
{
	ABPE_Weapon* LastOverlappingWeapon = OverlappingWeapon;
	OverlappingWeapon = Weapon;

	OnSetOverlappingWeapon(LastOverlappingWeapon);	
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::SetOverlappingSpawnPad(ABPE_SpawnPad* SpawnPad)
{
	ABPE_SpawnPad* LastOverlappingSpawnPad = OverlappingSpawnPad;
	OverlappingSpawnPad = SpawnPad;

	OnSetOverlappingSpawnPad(LastOverlappingSpawnPad);
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

	PlayMontage(SwapWeaponMontage, FName(), 2.0);

	if(IsValid(CurrentWeapon))
	{
		if(IsLocallyControlled())
		{
			OnCarriedAmmoChanged.Broadcast(CarriedAmmo);
			OnChangeCurrentWeaponDelegate.Broadcast(CurrentWeapon);
		}

		if(bIsUsingUltimate)
		{
			CurrentWeapon->SetCurrentColorType(EColorType::Multicolor);
		}
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
void ABPE_PlayerCharacter::OnRep_OverlappingSpawnPad(ABPE_SpawnPad* LastOverlappingSpawnPad)
{
	OnSetOverlappingSpawnPad(LastOverlappingSpawnPad);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::OnSetOverlappingSpawnPad(ABPE_SpawnPad* LastOverlappingSpawnPad)
{
	if(IsLocallyControlled())
	{
		if(IsValid(OverlappingSpawnPad))
		{
			OverlappingSpawnPad->SetWidgetVisibility(true);
		}
		if(IsValid(LastOverlappingSpawnPad))
		{
			LastOverlappingSpawnPad->SetWidgetVisibility(false);
		}	
	}
}

//----------------------------------------------------------------------------------------------------------------------
ABPE_Weapon* ABPE_PlayerCharacter::FindWeaponWithColorType(EColorType ColorType) const 
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IsValid(Inventory[i]) && Inventory[i]->GetDefaultColorType() == ColorType)
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

	if(IsLocallyControlled())
	{
		if(IsValid(CurrentWeapon))
		{
			InterpolateFieldOfView(DeltaSeconds);
		}
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

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABPE_PlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABPE_PlayerCharacter::Aim);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABPE_PlayerCharacter::EquipNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this,  &ABPE_PlayerCharacter::EquipPreviousWeapon);

	PlayerInputComponent->BindAction("Interact",IE_Pressed, this, &ABPE_PlayerCharacter::Interact);

	PlayerInputComponent->BindAction("Pause",IE_Pressed, this, &ABPE_PlayerCharacter::PauseGame);

	PlayerInputComponent->BindAction("Ultimate",IE_Pressed, this, &ABPE_PlayerCharacter::StartUltimate);

	PlayerInputComponent->BindAction("Reload",IE_Pressed, this, &ABPE_PlayerCharacter::Reload);
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, OverlappingSpawnPad, COND_OwnerOnly);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentWeapon);
	DOREPLIFETIME(ABPE_PlayerCharacter, bIsAiming);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, Inventory, COND_OwnerOnly);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentUltimateXP);
	DOREPLIFETIME(ABPE_PlayerCharacter, CurrentUltimateDuration);
	DOREPLIFETIME(ABPE_PlayerCharacter, bIsUsingUltimate);
	DOREPLIFETIME_CONDITION(ABPE_PlayerCharacter, CarriedAmmo, COND_OwnerOnly);
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

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::AreGameplayInputsEnabled() const
{
	if(IsValid(GetHealthComponent()) && IsValid(PlayerControllerReference))
	{
		return !GetHealthComponent()->IsDead() && PlayerControllerReference->AreGameplayInputsEnabled();
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
void ABPE_PlayerCharacter::AddUltimateXP(float XPAmount)
{
	if(bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPAmount, 0.0f, MaxUltimateXP);
	OnUltimateValueUpdated();
	
	bCanUseUltimate = FMath::IsNearlyEqual(CurrentUltimateXP, MaxUltimateXP);
}

//----------------------------------------------------------------------------------------------------------------------
bool ABPE_PlayerCharacter::TryReload()
{
	if(CarriedAmmo > 0)
	{
		Reload();
		return true;
	}
	return false;
}

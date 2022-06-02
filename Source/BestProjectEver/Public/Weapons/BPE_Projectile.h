// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class USoundCue;
class UParticleSystem;

UCLASS()
class BESTPROJECTEVER_API ABPE_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABPE_Projectile();

protected:

	//------------------------------------------------------------------------------------------------------------------
	// Components

	/** to apply damage and destroy on hit event */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	//------------------------------------------------------------------------------------------------------------------
	// Effects
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> ImpactSound;

	/** Initial speed of projectile */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float InitialSpeed;

	/** Damage applied on hit event */
	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** to perform effects */ 
	virtual void Destroyed() override;
};

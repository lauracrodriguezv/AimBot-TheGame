// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABPE_Projectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};

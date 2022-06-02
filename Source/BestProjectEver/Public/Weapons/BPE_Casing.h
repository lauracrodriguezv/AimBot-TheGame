// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_Casing.generated.h"

class USoundCue;

UCLASS()
class BESTPROJECTEVER_API ABPE_Casing : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABPE_Casing();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> CasingMesh;

	/** sound when casing hit something */ 
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundCue> ShellSound;

	/** Impulse when casing is spawned */ 
	UPROPERTY(EditAnywhere, Category="Movement")
	float ShellEjectionImpulse;
};
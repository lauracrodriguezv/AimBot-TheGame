// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_HealingArea.generated.h"

class ABPE_BaseCharacter;

UCLASS()
class BESTPROJECTEVER_API ABPE_HealingArea : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> AreaMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Materials")
	TObjectPtr<UMaterialInstanceDynamic> AreaMeshMaterialInstanceDynamic;

	UPROPERTY(EditAnywhere, Category="Healing")
	float HealingAmount;

	UPROPERTY(EditAnywhere, Category="Healing")
	float CooldownTime;

	FTimerHandle TimerHandle_Cooldown;
	
public:
	
	ABPE_HealingArea();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void HealCharacter(const ABPE_BaseCharacter* Character);

	/** Area can heal again */
	UFUNCTION()
	void RestoreHealingArea();

	UFUNCTION()
	void OnAreaMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};

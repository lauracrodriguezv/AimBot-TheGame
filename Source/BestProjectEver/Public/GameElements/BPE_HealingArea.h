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

	/** Area to heal overlapping character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> AreaMeshComponent;

	/** Health applied to heal overlapping character */
	UPROPERTY(EditAnywhere, Category="Healing")
	float HealingAmount;

	/** Time to reset healing area */
	UPROPERTY(EditAnywhere, Category="Healing")
	float CooldownTime;

	/** Handle for efficient management of restore healing area timer */
	FTimerHandle TimerHandle_Cooldown;
	
public:
	
	ABPE_HealingArea();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAreaMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** heal overlapping character */
	UFUNCTION()
	void HealCharacter(const ABPE_BaseCharacter* Character);
	
	/** area can heal again */
	UFUNCTION()
	void RestoreHealingArea();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BPE_SpawnPad.generated.h"

class UArrowComponent;
class USphereComponent;
class UNiagaraComponent;
class UWidgetComponent;
class UTimelineComponent;
class FOnTimelineFloat;

UCLASS()
class BESTPROJECTEVER_API ABPE_SpawnPad : public AActor
{
	GENERATED_BODY()

protected:

	//------------------------------------------------------------------------------------------------------------------
	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> SpawnPadBaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> SpawnPadRingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> HoloGridPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UArrowComponent> SpawnTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> ActivationTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> FX_SpawnPadActive;

	/** press I for pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWidgetComponent> InteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Effects)
	TObjectPtr<UTimelineComponent> RinMovementTimeline;

	//------------------------------------------------------------------------------------------------------------------
	// Spawner Data
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorToSpawnClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	TObjectPtr<AActor> CurrentActorSpawned;

	//------------------------------------------------------------------------------------------------------------------
	// Ring movement on spawn

	/** for visual effect when an actor is spawned */
	FOnTimelineFloat RingMovementTrack;

	/** for visual effect when an actor is spawned */
	UPROPERTY(EditAnywhere, Category= "Spawner")
	UCurveFloat* RingMovementCurve;
	
public:	
	
	ABPE_SpawnPad();

protected:
	
	virtual void BeginPlay() override;

	void InitializeReferences();

	UFUNCTION()
	void OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetWidgetVisibility(bool bShowWidget);

	/** [server] spawn an actor */
	void SpawnActor();

	/** active visual effects when an actor is spawned */
	void ActiveEffectsOnSpawn();

	/** set new ring position on spawned */
	void UpdateRingPosition(float DissolveValue);

public:

	//------------------------------------------------------------------------------------------------------------------
	//Spawner usage

	void OnInteract();
};
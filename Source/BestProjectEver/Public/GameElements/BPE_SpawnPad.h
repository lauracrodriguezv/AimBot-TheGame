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

	/** this component will move up and down each time an actor is spawned. Replicated for movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> SpawnPadRingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> HoloGridPlane;

	/** transform where actor will be spawned */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UArrowComponent> SpawnTransform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	/** area to enable interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> ActivationTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> FX_SpawnPadActive;

	/** press I for pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWidgetComponent> InteractWidget;

	/** timeline for ring movement when an actor is spawned */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	TObjectPtr<UTimelineComponent> RingMovementTimeline;

	//------------------------------------------------------------------------------------------------------------------
	// Spawner Data

	UPROPERTY(EditAnywhere, Category = "Spawner")
	FVector NewLocationMultiplier;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorToSpawnClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	TObjectPtr<AActor> CurrentActorSpawned;

	//------------------------------------------------------------------------------------------------------------------
	// Ring movement on spawn

	/** for visual effect when an actor is spawned */
	UPROPERTY(BlueprintReadOnly, Category= "Spawner")
	FOnTimelineFloat RingMovementTrack;

	/** for visual effect when an actor is spawned */
	UPROPERTY(EditAnywhere, Category= "Spawner")
	UCurveFloat* RingMovementCurve;
	
public:	
	
	ABPE_SpawnPad();

protected:
	
	virtual void BeginPlay() override;

	void InitializeReferences();

	/** [server] called when something starts overlaps ActivationTrigger component */
	UFUNCTION()
	void OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** [server] called when something ends overlaps ActivationTrigger component */
	UFUNCTION()
	void OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//------------------------------------------------------------------------------------------------------------------
	//Spawner usage helpers
	
	/** [server] spawn an actor */
	void SpawnActor();

	/** [server] active visual effects when an actor is spawned */
	void ActiveEffectsOnSpawn();

	/** [server] set new ring position on spawned */
	UFUNCTION()
	void UpdateRingPosition(float Value);

public:

	//------------------------------------------------------------------------------------------------------------------
	//Spawner usage

	/** [client and server] called when character start or end overlapping ActivationTrigger if it is locally controlled*/
	void SetWidgetVisibility(bool bShowWidget);

	/** [server] called when interaction is enabled and character press I to interact */
	void OnInteract();
};
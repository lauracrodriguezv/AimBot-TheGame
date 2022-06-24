// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPE_GameStarter.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class BESTPROJECTEVER_API ABPE_GameStarter : public AActor
{
	GENERATED_BODY()

protected:

	//------------------------------------------------------------------------------------------------------------------
	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> PortalFrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> PortalPlaneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> ActivationTrigger;

	/** "go here to start the game" */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWidgetComponent> InformationWidget;

	UPROPERTY(ReplicatedUsing=OnRep_TravelSucceed)
	uint8 bTravelSucceeded : 1;
	
public:	
	
	ABPE_GameStarter();

protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	UFUNCTION()
	void OnRep_TravelSucceed();

	/** [server] called when something starts overlaps ActivationTrigger component */
	UFUNCTION()
	void OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
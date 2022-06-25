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
	TObjectPtr<UStaticMeshComponent> PortalPlaneMesh;

	/** Trigger to start map travel */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> ActivationTrigger;

	/** "go here to start the game" */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWidgetComponent> InformationWidget;

public:	
	
	ABPE_GameStarter();

protected:

	virtual void BeginPlay() override;

	void InitializeReferences();

	/** [server] called when something starts overlaps ActivationTrigger component */
	UFUNCTION()
	void OnActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** ask the server to start travel to gameplay map */
	void TravelToGameplayMap();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_TravelToGameplayMap();
};
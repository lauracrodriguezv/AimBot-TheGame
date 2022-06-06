// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPE_PlayerController.generated.h"

class ABPE_HUD;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API ABPE_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category="UI")
	ABPE_HUD* HUD;
	
protected:

	virtual void BeginPlay() override;
};

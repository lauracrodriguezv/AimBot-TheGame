// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABP_PlayerController.generated.h"

class ABPE_HUD;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API AABP_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category="UI")
	ABPE_HUD* PlayerHUD;
	
protected:

	virtual void BeginPlay() override;
	
};

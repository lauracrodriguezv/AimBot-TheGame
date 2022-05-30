// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BPE_BaseCharacter.generated.h"

UCLASS()
class BESTPROJECTEVER_API ABPE_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABPE_BaseCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};

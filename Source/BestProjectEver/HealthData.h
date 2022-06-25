#pragma once

#include "HealthData.generated.h"

USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_BODY()

	float CurrentHealth;

	float MaxHealth;

	UPROPERTY()
	TObjectPtr<AActor> AffectedActor;

	UPROPERTY()
	TObjectPtr<AController> InstigatedBy;

	UPROPERTY()
	TObjectPtr<AActor> HealthChangeCauser;

	FHealthData() : CurrentHealth(0.0f), MaxHealth(100.0f), AffectedActor(nullptr), InstigatedBy(nullptr), HealthChangeCauser(nullptr) {}
	FHealthData(const float Health, const float MaxHealth, AActor* DamagedActor, AController* DamagedController, AActor* Causer) :
		CurrentHealth(Health), MaxHealth(MaxHealth), AffectedActor(DamagedActor), InstigatedBy(DamagedController), HealthChangeCauser(Causer) {}
};
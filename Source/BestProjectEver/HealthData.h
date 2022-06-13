#pragma once

#include "HealthData.generated.h"

USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_BODY()

	float CurrentHealth;

	float MaxHealth;

	UPROPERTY()
	TObjectPtr<AActor> DamagedActor;

	UPROPERTY()
	TObjectPtr<AController> InstigatedBy;

	UPROPERTY()
	TObjectPtr<AActor> DamageCauser;

	FHealthData() : CurrentHealth(0.0f), MaxHealth(100.0f), DamagedActor(nullptr), InstigatedBy(nullptr), DamageCauser(nullptr) {}
	FHealthData(const float Health, const float MaxHealth, AActor* DamagedActor, AController* DamagedController, AActor* Causer) :
		CurrentHealth(Health), MaxHealth(MaxHealth), DamagedActor(DamagedActor), InstigatedBy(DamagedController), DamageCauser(Causer) {}
};
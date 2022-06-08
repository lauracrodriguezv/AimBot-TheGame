// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPE_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangeSignature, class UBPE_HealthComponent*, CurrentHealthComponent,
	float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BESTPROJECTEVER_API UBPE_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Health Component")
	uint8 bIsDead : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health Component")
	float CurrentHealth;
	
public:	
	
	UBPE_HealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser );

	UFUNCTION()
	void OnRep_Health();

	void OnHealthChange();

	UFUNCTION()
	void OnRep_IsDead();

	void OnIsDead();

public:	

	bool IsDead() const { return bIsDead; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

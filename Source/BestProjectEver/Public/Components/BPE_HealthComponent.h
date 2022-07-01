// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BestProjectEver/HealthData.h"
#include "BPE_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, const FHealthData&, HealthData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BESTPROJECTEVER_API UBPE_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Health Component")
	uint8 bIsDead : 1;

	/** character max health, 100 by default */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health Component")
	float CurrentHealth;

	/** actor who applied damage in any way to this actor */
	UPROPERTY(Replicated, Transient)
	TObjectPtr<AActor> DamageCauserActor;
	
public:	
	
	UBPE_HealthComponent();

	/** [server and client] called when health changes */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChangeDelegate;

	/** [server and client] called when character is dead */
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;

protected:
	
	virtual void BeginPlay() override;

	/** [server] update heath data */
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser );

	/** [client] current health changed rep handler */
	UFUNCTION()
	void OnRep_Health();

	/** [server and client] current health changed handler */
	void OnHealthChange();

	/** [client] character is death rep handler */
	UFUNCTION()
	void OnRep_IsDead();

	/** [server and client] character is death handler */
	void OnIsDead();

public:	

	/** Increase current health
	 * @param HealAmount Health to add to current health
	 * @param InstigatedBy Actor who healed owner
	 */
	void Heal(const float HealAmount, AActor* InstigatedBy);
	
	bool IsDead() const { return bIsDead; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	AActor* GetDamageCauser() const { return DamageCauserActor; }
};

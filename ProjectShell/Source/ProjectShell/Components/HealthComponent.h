// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSHELL_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

    //virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:	
    UFUNCTION(BlueprintCallable, Category = "Death")
        void SetDeathFunction(class ABaseTank* tank);

    UFUNCTION(BlueprintCallable, Category = "Damage")
        void TakeDamage(float damage);

    UFUNCTION(BlueprintCallable, Category = "Heal")
        void IncreaseHealth(float health, bool allowOverheal = false);

    UFUNCTION(BlueprintCallable, Category = "Heal")
        void IncreaseHealthToFull(bool reduceHealthIfOver = false);

    UFUNCTION(BlueprintCallable, Category = "Health")
        const bool CheckDeath();

protected:
    DECLARE_DELEGATE(OnDeath);
    OnDeath DeathFunction;

    UPROPERTY(Category = "Health", EditDefaultsOnly, BlueprintReadWrite)
        float MaximumHealth;
    
    float CurrentHealth;

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE float GetMaxHealth() const { return MaximumHealth; }
    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE float GetHealth() const { return CurrentHealth; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Defaults
    MaximumHealth = 5.f;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaximumHealth;
}

//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

void UHealthComponent::TakeDamage(float damage)
{
    CurrentHealth -= damage;
}

void UHealthComponent::IncreaseHealth(float health, bool allowOverheal)
{
    CurrentHealth += health;
    if (!allowOverheal && CurrentHealth > MaximumHealth)
    {
        CurrentHealth = MaximumHealth;
    }
}

void UHealthComponent::IncreaseHealthToFull(bool reduceHealthIfOver)
{
    if (CurrentHealth < MaximumHealth || reduceHealthIfOver)
    {
        CurrentHealth = MaximumHealth;
    }
}

const bool UHealthComponent::CheckDeath()
{
    bool dead = false;
    if (CurrentHealth <= 0)
    {
        dead = true;
    }
    return dead;
}

const bool UHealthComponent::TakeDamageAndCheckDeath(float damage)
{
    TakeDamage(damage);
    return CheckDeath();
}


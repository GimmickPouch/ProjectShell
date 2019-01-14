// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Defaults
    _maximumHealth = 10.f;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    _currentHealth = _maximumHealth;
}

//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

void UHealthComponent::TakeDamage(float damage)
{
    _currentHealth -= damage;
}

const bool UHealthComponent::CheckDeath()
{
    bool dead = false;
    if (_currentHealth <= 0)
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


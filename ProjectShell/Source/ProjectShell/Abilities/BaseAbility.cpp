// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbility.h"
#include "Engine/World.h"

UBaseAbility::UBaseAbility()
{
    bCanUseAbility = true;
}

void UBaseAbility::ActivateAbility()
{
    if (bCanUseAbility)
    {
        AbilityAction();
        if (AbilityCooldownSeconds > 0)
        {
            bCanUseAbility = false;
            GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UBaseAbility::AbilityCooldownExpired, AbilityCooldownSeconds);
        }
    }
}

void UBaseAbility::AbilityAction_Implementation()
{

}

void UBaseAbility::AbilityCooldownExpired()
{
    bCanUseAbility = true;
}
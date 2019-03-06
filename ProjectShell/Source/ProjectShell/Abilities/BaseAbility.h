// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "BaseAbility.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSHELL_API UBaseAbility : public UActorComponent
{
    GENERATED_BODY()

public:
    UBaseAbility();

public:
    void ActivateAbility();

protected:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Abilities")
        void AbilityAction();
    virtual void AbilityAction_Implementation();

    void AbilityCooldownExpired();

protected:
    UPROPERTY(Category = "Cooldown", EditAnywhere, BlueprintReadWrite)
        float AbilityCooldownSeconds;

    FTimerHandle CooldownTimerHandle;

    bool bCanUseAbility;
};

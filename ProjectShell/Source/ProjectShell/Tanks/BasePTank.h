// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "BasePTank.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHELL_API ABasePTank : public AWheeledVehicle
{
    GENERATED_BODY()

public:
    ABasePTank();
    
protected:

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveForward(float val);
    void MoveRight(float val);
    
    void UpdateTankLocation();
};

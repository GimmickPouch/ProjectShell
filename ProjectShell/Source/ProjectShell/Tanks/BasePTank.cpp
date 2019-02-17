// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePTank.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Wheels/BaseTankWheelFront.h"
#include "Wheels/BaseTankWheelBack.h"

ABasePTank::ABasePTank()
{

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> carMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
    GetMesh()->SetSkeletalMesh(carMesh.Object);

    static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
    GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

    // Simulation
    UWheeledVehicleMovementComponent4W* vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

    check(vehicle4W->WheelSetups.Num() == 4);

    vehicle4W->WheelSetups[0].WheelClass = UBaseTankWheelFront::StaticClass();
    vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
    vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

    vehicle4W->WheelSetups[1].WheelClass = UBaseTankWheelFront::StaticClass();
    vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
    vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

    vehicle4W->WheelSetups[2].WheelClass = UBaseTankWheelBack::StaticClass();
    vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
    vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

    vehicle4W->WheelSetups[3].WheelClass = UBaseTankWheelBack::StaticClass();
    vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
    vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);
}

void ABasePTank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Set up gameplay key bindings
    //PlayerInputComponent->BindAxis(kMoveForwardBinding);
    //PlayerInputComponent->BindAxis(kMoveRightBinding);
    //PlayerInputComponent->BindAxis(kFireForwardBinding);
    //PlayerInputComponent->BindAxis(kFireRightBinding);

    PlayerInputComponent->BindAxis("MoveForward");//, this, &ABasePTank::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight");// , this, &ABasePTank::MoveRight);

    //PlayerInputComponent->BindAction("FireShot", IE_Pressed, this, &ABaseTank::FireShot);

    //PlayerInputComponent->BindAction("SpecialAbility", IE_Pressed, this, &ABaseTank::ActivateSpecialAbility);
    //PlayerInputComponent->BindAction("DefensiveAbility", IE_Pressed, this, &ABaseTank::ActivateDefensiveAbility);
}

void ABasePTank::MoveForward(float val)
{
    //GetVehicleMovement()->SetThrottleInput(val);
}

void ABasePTank::MoveRight(float val)
{
    //GetVehicleMovement()->SetSteeringInput(val);
}

void ABasePTank::Tick(float DeltaSeconds)
{
    UpdateTankLocation();
    //UpdateCannonRotation();
}

void ABasePTank::UpdateTankLocation()
{
    // Find movement direction
    const float forwardValue = GetInputAxisValue("MoveForward");
    const float rightValue = GetInputAxisValue("MoveRight");

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector moveDirection = FVector(forwardValue, rightValue, 0.f).GetClampedToMaxSize(1.0f);

    // Calculate  movement
    //const FVector movement = moveDirection * 1000.f * GetWorld()->GetDeltaSeconds();

    // If non-zero size, move this actor
    if (moveDirection.SizeSquared() != 0.0f)
    {
        this->SetActorRotation(moveDirection.Rotation(), ETeleportType::TeleportPhysics);
        //GetVehicleMovement()->Velocity = FVector(100.f, 100.f, 10.f);
        //GetVehicleMovement()->UpdateComponentVelocity();
        //const FRotator newRotation = movement.Rotation();
        //FHitResult hit(1.f);
        //RootComponent->MoveComponent(FVector(0.f,0.f,0.f), newRotation, true, &hit);

        GetVehicleMovement()->SetThrottleInput(1);

        // Reset cannon rotation as it's parented to the tank
        //_cannonBase->SetWorldRotation(_cannonRotation);
    }
    else
    {
        GetVehicleMovement()->SetThrottleInput(0);
    }
}

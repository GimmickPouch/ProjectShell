// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePhysicsTank.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
ABasePhysicsTank::ABasePhysicsTank()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    TankMainBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    TankMainBodyMesh->SetSimulatePhysics(true);
    RootComponent = TankMainBodyMesh;
}

// Called when the game starts or when spawned
void ABasePhysicsTank::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ABasePhysicsTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePhysicsTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ABasePhysicsTank::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ABasePhysicsTank::MoveRight);
}

void ABasePhysicsTank::MoveForward(float val)
{
    // AddLocalWorldOffset
    FHitResult hitResult;
    AddActorWorldOffset(FVector(val * 20.f, 0.f, 0.f), false, &hitResult);

    if (hitResult.bBlockingHit)
    {
        DrawDebugLine(
            GetWorld(),
            hitResult.Location,
            hitResult.Location + FVector(0.f, 0.f, 300.f),
            FColor(255, 0, 0),
            false, 3, 0,
            10
        );
    }
}

void ABasePhysicsTank::MoveRight(float val)
{
    // AddLocalWorldOffset
    FHitResult hitResult;
    AddActorWorldOffset(FVector(0.f, val * 20.f, 0.f), false, &hitResult);

    if (hitResult.bBlockingHit)
    {
        DrawDebugLine(
            GetWorld(),
            hitResult.Location,
            hitResult.Location + FVector(0.f, 0.f, 300.f),
            FColor(255, 0, 0),
            false, 3, 0,
            10
        );
    }
}
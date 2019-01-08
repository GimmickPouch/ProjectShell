// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTank.h"
#include "ProjectShellProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

const FName ABaseTank::kMoveForwardBinding("MoveForward");
const FName ABaseTank::kMoveRightBinding("MoveRight");
const FName ABaseTank::kFireForwardBinding("FireForward");
const FName ABaseTank::kFireRightBinding("FireRight");

ABaseTank::ABaseTank()
{
    // Create the mesh component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    _tankStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    _tankStaticMesh->SetupAttachment(RootComponent);
    _tankStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> tankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    if (tankMesh.Succeeded())
    {
        _tankStaticMesh->SetStaticMesh(tankMesh.Object);
    }

    _cannonBase = CreateDefaultSubobject<USceneComponent>(TEXT("CannonPivot"));
    _cannonBase->SetupAttachment(_tankStaticMesh);

    _cannonStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
    _cannonStaticMesh->SetupAttachment(_cannonBase);

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> fireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    if (fireAudio.Succeeded())
    {
        _fireSound = fireAudio.Object;
    }

    // Movement
    _moveSpeed = 1000.0f;

    // Weapon
    _bulletSpawnOffset = FVector(90.f, 0.f, 0.f);
    _fireRate = 0.1f;
    _canFire = true;
    _cannonRotation = FRotator();
}

void ABaseTank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Set up gameplay key bindings
    PlayerInputComponent->BindAxis(kMoveForwardBinding);
    PlayerInputComponent->BindAxis(kMoveRightBinding);
    PlayerInputComponent->BindAxis(kFireForwardBinding);
    PlayerInputComponent->BindAxis(kFireRightBinding);

    PlayerInputComponent->BindAction("FireShot", IE_Pressed, this, &ABaseTank::FireShot);
}

void ABaseTank::Tick(float DeltaSeconds)
{
    UpdateTankLocation();
    UpdateCannonRotation();
}

void ABaseTank::UpdateTankLocation()
{
    // Find movement direction
    const float forwardValue = GetInputAxisValue(kMoveForwardBinding);
    const float rightValue = GetInputAxisValue(kMoveRightBinding);

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector moveDirection = FVector(forwardValue, rightValue, 0.f).GetClampedToMaxSize(1.0f);

    // Calculate  movement
    const FVector movement = moveDirection * _moveSpeed * GetWorld()->GetDeltaSeconds();

    // If non-zero size, move this actor
    if (movement.SizeSquared() > 0.0f)
    {
        const FRotator newRotation = movement.Rotation();
        FHitResult hit(1.f);
        RootComponent->MoveComponent(movement, newRotation, true, &hit);

        if (hit.IsValidBlockingHit())
        {
            const FVector normal2D = hit.Normal.GetSafeNormal2D();
            const FVector deflection = FVector::VectorPlaneProject(movement, normal2D) * (1.f - hit.Time);
            RootComponent->MoveComponent(deflection, newRotation, true);
        }

        // Reset cannon rotation as it's parented to the tank
        _cannonBase->SetWorldRotation(_cannonRotation);
    }
}

void ABaseTank::UpdateCannonRotation()
{
    const float fireForwardValue = GetInputAxisValue(kFireForwardBinding);
    const float fireRightValue = GetInputAxisValue(kFireRightBinding);
    const FVector fireDirection = FVector(fireForwardValue, fireRightValue, 0.f);

    // If we are pressing cannon aim stick in a direction
    if (fireDirection.SizeSquared() > 0.0f)
    {
        _cannonRotation = fireDirection.Rotation();
        _cannonBase->SetWorldRotation(_cannonRotation);
    }
}

void ABaseTank::FireShot()
{
    FireShot(_cannonRotation.Vector());
}

void ABaseTank::FireShot(FVector fireDirection)
{
    // If we can fire and we are pressing fire stick in a direction
    if (_canFire && fireDirection.SizeSquared() > 0.0f)
    {
        UWorld* const world = GetWorld();
        if (world)
        {
            const FRotator fireRotation = fireDirection.Rotation();
            // Spawn projectile at an offset from this pawn
            const FVector spawnLocation = GetActorLocation() + fireRotation.RotateVector(_bulletSpawnOffset);
            // Spawn the projectile
            world->SpawnActor<AProjectShellProjectile>(spawnLocation, fireRotation);
            world->GetTimerManager().SetTimer(_fireCooldownTimerHandle, this, &ABaseTank::ShotCooldownExpired, _fireRate);

            // Try and play the sound if specified
            if (_fireSound != nullptr)
            {
                UGameplayStatics::PlaySoundAtLocation(this, _fireSound, GetActorLocation());
            }

            _canFire = false;
        }
    }
}

void ABaseTank::ShotCooldownExpired()
{
    _canFire = true;
}
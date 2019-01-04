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
}

void ABaseTank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Set up gameplay key bindings
    PlayerInputComponent->BindAxis(kMoveForwardBinding);
    PlayerInputComponent->BindAxis(kMoveRightBinding);
    PlayerInputComponent->BindAxis(kFireForwardBinding);
    PlayerInputComponent->BindAxis(kFireRightBinding);
}

void ABaseTank::Tick(float DeltaSeconds)
{
    // Find movement direction
    const float ForwardValue = GetInputAxisValue(kMoveForwardBinding);
    const float RightValue = GetInputAxisValue(kMoveRightBinding);

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

    // Calculate  movement
    const FVector Movement = MoveDirection * _moveSpeed * DeltaSeconds;

    // If non-zero size, move this actor
    if (Movement.SizeSquared() > 0.0f)
    {
        const FRotator NewRotation = Movement.Rotation();
        FHitResult Hit(1.f);
        RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

        if (Hit.IsValidBlockingHit())
        {
            const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
            RootComponent->MoveComponent(Deflection, NewRotation, true);
        }
    }

    // Create fire direction vector
    const float FireForwardValue = GetInputAxisValue(kFireForwardBinding);
    const float FireRightValue = GetInputAxisValue(kFireRightBinding);
    const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

    // Try and fire a shot
    FireShot(FireDirection);
}

void ABaseTank::FireShot(FVector FireDirection)
{
    // If it's ok to fire again
    if (_canFire == true)
    {
        // If we are pressing fire stick in a direction
        if (FireDirection.SizeSquared() > 0.0f)
        {
            const FRotator FireRotation = FireDirection.Rotation();
            // Spawn projectile at an offset from this pawn
            const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(_bulletSpawnOffset);

            UWorld* const World = GetWorld();
            if (World != NULL)
            {
                // spawn the projectile
                World->SpawnActor<AProjectShellProjectile>(SpawnLocation, FireRotation);
            }

            _canFire = false;
            World->GetTimerManager().SetTimer(_fireCooldownTimerHandle, this, &ABaseTank::ShotTimerExpired, _fireRate);

            // try and play the sound if specified
            if (_fireSound != nullptr)
            {
                UGameplayStatics::PlaySoundAtLocation(this, _fireSound, GetActorLocation());
            }

            _canFire = false;
        }
    }
}

void ABaseTank::ShotTimerExpired()
{
    _canFire = true;
}
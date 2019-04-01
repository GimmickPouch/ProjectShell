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
#include "Projectiles/BaseProjectile.h"
#include "Components/HealthComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Abilities/BaseAbility.h"
#include "Engine.h" // For debug on screen

const FName ABaseTank::kMoveForwardBinding("MoveForward");
const FName ABaseTank::kMoveRightBinding("MoveRight");
const FName ABaseTank::kFireForwardBinding("FireForward");
const FName ABaseTank::kFireRightBinding("FireRight");

ABaseTank::ABaseTank()
{
    // Create the mesh component
    TankMainBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    TankMainBodyMesh->SetSimulatePhysics(true);
    RootComponent = TankMainBodyMesh;
    TankMainBodyMesh->bGenerateOverlapEvents = true;

    CannonBase = CreateDefaultSubobject<USceneComponent>(TEXT("CannonPivot"));
    CannonBase->SetupAttachment(TankMainBodyMesh);

    CannonStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
    CannonStaticMesh->SetupAttachment(CannonBase);

    static ConstructorHelpers::FObjectFinder<UClass> testProjectileBP(TEXT("Class'/Game/Blueprints/Projectiles/TestProjectile.TestProjectile_C'"));
    if (testProjectileBP.Succeeded())
    {
        DefaultProjectile = testProjectileBP.Object;
    }
    
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->SetDeathFunction(this);

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> fireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    if (fireAudio.Succeeded())
    {
        FireSound = fireAudio.Object;
    }

    // Movement
    MoveSpeed = 1000.0f;
    AccelerationSeconds = 0.5f;

    // Shooting
    BulletSpawnOffset = FVector(120.f, 0.f, 0.f);
    FireRate = 1.f;
    bLockCannonRotation = true;
}

void ABaseTank::BeginPlay()
{
    Super::BeginPlay();
    TankMainBodyMesh->OnComponentHit.AddDynamic(this, &ABaseTank::OnHit);

    World = GetWorld();

    // Shooting
    bCanFire = true;
    CannonRotation = GetActorRotation();

    // Abilities
    if (SpecialAbilityEquipped)
    {
        SpecialAbilityInstance = NewObject<UBaseAbility>(this, SpecialAbilityEquipped);
    }

    if (DefensiveAbilityEquipped)
    {
        DefensiveAbilityInstance = NewObject<UBaseAbility>(this, DefensiveAbilityEquipped);
    }

    bCanUseSpecialAbility = true;
    bCanUseDefensiveAbility = true;
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

    PlayerInputComponent->BindAction("SpecialAbility", IE_Pressed, this, &ABaseTank::ActivateSpecialAbility);
    PlayerInputComponent->BindAction("DefensiveAbility", IE_Pressed, this, &ABaseTank::ActivateDefensiveAbility);
}

void ABaseTank::Tick(float DeltaSeconds)
{
    UpdateTankLocation();
    UpdateCannonRotation();

    if (bCanFire && CannonBase) // TEMP
    {
        DrawDebugLine(World, CannonStaticMesh->GetComponentLocation(), CannonStaticMesh->GetComponentLocation() + (CannonBase->GetComponentRotation().Vector() * 300), FColor::Red, false, -1.0f, 0, 10.0f);
    }
}

void ABaseTank::UpdateTankLocation()
{
    // Find movement direction
    const float forwardValue = GetInputAxisValue(kMoveForwardBinding);
    const float rightValue = GetInputAxisValue(kMoveRightBinding);

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    FVector moveDirection = FVector(forwardValue, rightValue, 0.f).GetClampedToMaxSize(1.0f);

    // TODO - Only grab this once. This is inefficient
    MainCameraYawRotation = FRotator(0.f, UGameplayStatics::GetPlayerController(World, 0)->PlayerCameraManager->GetCameraRotation().Yaw, 0.f);

    // Rotate vector based on the camera rotation
    moveDirection = MainCameraYawRotation.RotateVector(moveDirection);

    // Calculate  movement
    AccelerationLerpSeconds += World->GetDeltaSeconds();
    AccelerationLerpSeconds = FMath::Clamp(AccelerationLerpSeconds, 0.f, AccelerationSeconds);
    const float currentAccelerationNormalizedPercentage = AccelerationSeconds == 0 ? 1 : AccelerationLerpSeconds / AccelerationSeconds;
    const FVector movement = moveDirection * (MoveSpeed * currentAccelerationNormalizedPercentage) * GetWorld()->GetDeltaSeconds();

    // If non-zero size, move this actor
    if (movement.SizeSquared() > 0.0f)
    {
        const FRotator newRotation = movement.Rotation();
        RootComponent->MoveComponent(movement, newRotation, false);
    }
    else
    {
        AccelerationLerpSeconds = 0;
    }
}

void ABaseTank::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        if (OtherActor->IsA(ABaseProjectile::StaticClass()))
        {
            ABaseProjectile* projectile = Cast<ABaseProjectile>(OtherActor);
            if (projectile && HealthComponent)
            {
                HealthComponent->TakeDamage(projectile->GetDamage());
            }
        }
    }
}

void ABaseTank::UpdateCannonRotation()
{
    const float fireForwardValue = GetInputAxisValue(kFireForwardBinding);
    const float fireRightValue = GetInputAxisValue(kFireRightBinding);
    FVector fireDirection = FVector(fireForwardValue, fireRightValue, 0.f);

    fireDirection = MainCameraYawRotation.RotateVector(fireDirection);

    bool updateCannonRotation = fireDirection.SizeSquared() > 0.0f;

    // If we are pressing cannon aim stick in a direction
    if (updateCannonRotation)
    {
        CannonRotation = fireDirection.Rotation();
    }

    if (updateCannonRotation || bLockCannonRotation)
    {
        // Reset cannon rotation as it's parented to the tank
        CannonBase->SetWorldRotation(CannonRotation);
    }
}

void ABaseTank::FireShot()
{
    if (CannonBase)
    {
        FireShot(CannonBase->GetComponentRotation().Vector());
    }
}

void ABaseTank::FireShot(FVector fireDirection)
{
    // If we can fire and we are pressing fire stick in a direction
    if (bCanFire && fireDirection.SizeSquared() > 0.0f)
    {
        if (World && DefaultProjectile)
        {
            const FRotator fireRotation = fireDirection.Rotation();
            // Spawn projectile at an offset from this pawn
            const FVector spawnLocation = TankMainBodyMesh->GetComponentLocation() + fireRotation.RotateVector(BulletSpawnOffset);
            // Spawn the projectile
            const FActorSpawnParameters spawnParams = FActorSpawnParameters();
            World->SpawnActor<ABaseProjectile>(DefaultProjectile, spawnLocation, fireRotation, spawnParams);
            World->GetTimerManager().SetTimer(FireCooldownTimerHandle, this, &ABaseTank::ShotCooldownExpired, FireRate);

            // Try and play the sound if specified
            if (FireSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
            }

            bCanFire = false;
        }
    }
}

void ABaseTank::ShotCooldownExpired()
{
    bCanFire = true;
}

void ABaseTank::PreDeath_Implementation()
{

}

void ABaseTank::OnDeath()
{
    PreDeath();
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("DEATH"));
    Destroy();
}

void ABaseTank::ActivateSpecialAbility()
{
    if (bCanUseSpecialAbility && SpecialAbilityInstance)
    {
        SpecialAbilityInstance->ActivateAbility();
    }
}

void ABaseTank::ActivateDefensiveAbility()
{
    if (bCanUseDefensiveAbility && DefensiveAbilityInstance)
    {
        DefensiveAbilityInstance->ActivateAbility();
    }
}
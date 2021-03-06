// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"


ABaseProjectile::ABaseProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    // Static reference to the mesh to use for the projectile
    static ConstructorHelpers::FObjectFinder<UStaticMesh> projectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

    // Create mesh component for the projectile sphere
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    if (projectileMeshAsset.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(projectileMeshAsset.Object);
    }
    ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
    ProjectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
    ProjectileMesh->SetupAttachment(RootComponent);
    RootComponent = ProjectileMesh;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 6000.f;
    ProjectileMovement->MaxSpeed = 6000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

    // Defaults
    Damage = 1.f;
    DestroyAfterSeconds = 0;

    ShootingTank = nullptr;
}

void ABaseProjectile::BeginPlay()
{
    Super::BeginPlay();

    LifeSeconds = 0;
}

void ABaseProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LifeSeconds += DeltaTime;

    if (DestroyAfterSeconds > 0 && LifeSeconds >= DestroyAfterSeconds)
    {
        Destroy();
    }
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != ((AActor*)GetShootingTank()))
    {
        if (OtherActor != NULL && OtherActor != this && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
        }

        Destroy();
    }
}

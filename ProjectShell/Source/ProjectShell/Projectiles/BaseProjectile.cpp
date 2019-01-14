// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"


ABaseProjectile::ABaseProjectile()
{
    // Static reference to the mesh to use for the projectile
    static ConstructorHelpers::FObjectFinder<UStaticMesh> projectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

    // Create mesh component for the projectile sphere
    _projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    if (projectileMeshAsset.Succeeded())
    {
        _projectileMesh->SetStaticMesh(projectileMeshAsset.Object);
    }
    _projectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
    _projectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
    _projectileMesh->SetupAttachment(RootComponent);
    RootComponent = _projectileMesh;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    _projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    _projectileMovement->UpdatedComponent = _projectileMesh;
    _projectileMovement->InitialSpeed = 3000.f;
    _projectileMovement->MaxSpeed = 3000.f;
    _projectileMovement->bRotationFollowsVelocity = true;
    _projectileMovement->bShouldBounce = true;
    _projectileMovement->ProjectileGravityScale = 0.f; // No gravity

    // Defaults
    _damage = 1.f;
}

void ABaseProjectile::BeginPlay()
{
    Super::BeginPlay();
    
}

void ABaseProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
    }

    Destroy();
}

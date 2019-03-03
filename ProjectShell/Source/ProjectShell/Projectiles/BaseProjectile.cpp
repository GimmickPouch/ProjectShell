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

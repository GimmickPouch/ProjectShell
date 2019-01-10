// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"


ABaseProjectile::ABaseProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    _bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    _bulletMesh->SetupAttachment(RootComponent);
}

void ABaseProjectile::BeginPlay()
{
    Super::BeginPlay();
    
}

void ABaseProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


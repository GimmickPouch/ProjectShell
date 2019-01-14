// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class PROJECTSHELL_API ABaseProjectile : public AActor
{
    GENERATED_BODY()
    
public:	
    ABaseProjectile();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
        UStaticMeshComponent* _projectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* _projectileMovement;

public:
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return _projectileMesh; }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return _projectileMovement; }
    
};

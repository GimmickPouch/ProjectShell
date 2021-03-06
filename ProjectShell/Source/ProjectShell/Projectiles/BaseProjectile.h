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
    ABaseProjectile(class ABaseTank* owner);

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
        UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* ProjectileMovement;

public:
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
    // Variables
    UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadWrite)
        float Damage;

    UPROPERTY(Category = "Shooting", EditDefaultsOnly, BlueprintReadWrite)
        float DestroyAfterSeconds;

    float LifeSeconds;

    class ABaseTank* ShootingTank;

public:
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE float GetDamage() const { return Damage; }

    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE class ABaseTank* GetShootingTank() const { return ShootingTank; }
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        void SetShootingTank(class ABaseTank* shootingTank) { ShootingTank = shootingTank; };
};

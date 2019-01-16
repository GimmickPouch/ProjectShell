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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
        UStaticMeshComponent* _projectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* _projectileMovement;

public:
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return _projectileMesh; }
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return _projectileMovement; }

protected:
    // Variables
    UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadWrite)
        float _damage;
    
public:
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
        FORCEINLINE float GetDamage() const { return _damage; }
};

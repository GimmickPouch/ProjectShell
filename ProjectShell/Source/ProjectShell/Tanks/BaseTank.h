// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseTank.generated.h"

UCLASS()
class PROJECTSHELL_API ABaseTank : public APawn
{
    GENERATED_BODY()

public:
    ABaseTank();

protected:
    //virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void UpdateTankLocation();

    // Collision
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
        class UBoxComponent* _collisionBox;

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Shooting
    void UpdateCannonRotation();
    void FireShot();
    void FireShot(FVector FireDirection);
    void ShotCooldownExpired();

    // Abilities
protected:
    void ActivateSpecialAbility();
    void ActivateDefensiveAbility();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Abilities")
        void SpecialAbilityAction();
    virtual void SpecialAbilityAction_Implementation();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Abilities")
        void DefensiveAbilityAction();
    virtual void DefensiveAbilityAction_Implementation();

protected:
    static const FName kMoveForwardBinding;
    static const FName kMoveRightBinding;
    static const FName kFireForwardBinding;
    static const FName kFireRightBinding;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class UStaticMeshComponent* _tankStaticMesh;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class USceneComponent* _cannonBase;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class UStaticMeshComponent* _cannonStaticMesh;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
        float _moveSpeed;

    // Health
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly)
        class UHealthComponent* _healthComponent;

    // Shooting
    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        UClass* _defaultProjectile;

    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        FVector _bulletSpawnOffset;

    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        float _fireRate;

    bool _canFire;
    FRotator _cannonRotation;
    FTimerHandle _fireCooldownTimerHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
    FORCEINLINE class UStaticMeshComponent* GetStaticMesh() const { return _tankStaticMesh; }
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return _healthComponent; }
    
    // Audio 
protected:
    UPROPERTY(Category = Audio, EditDefaultsOnly, BlueprintReadWrite)
        class USoundBase* _fireSound;
};

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
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void UpdateTankLocation();

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

    void SpecialAbilityCooldownExpired();
    void DefensiveAbilityCooldownExpired();

    FTimerHandle SpecialAbilityTimerHandle;
    FTimerHandle DefensiveAbilityTimerHandle;

    UPROPERTY(Category = Abilities, EditAnywhere, BlueprintReadWrite)
        float SpecialAbilityCooldownSeconds;

    UPROPERTY(Category = Abilities, EditAnywhere, BlueprintReadWrite)
        float DefensiveAbilityCooldownSeconds;

    bool bCanUseSpecialAbility;
    bool bCanUseDefensiveAbility;

protected:
    static const FName kMoveForwardBinding;
    static const FName kMoveRightBinding;
    static const FName kFireForwardBinding;
    static const FName kFireRightBinding;

    UWorld* World;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class UStaticMeshComponent* TankMainBodyMesh;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class USceneComponent* CannonBase;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly)
        class UStaticMeshComponent* CannonStaticMesh;

    UPROPERTY(Category = Movement, EditAnywhere, BlueprintReadWrite)
        float MoveSpeed;
    UPROPERTY(Category = Movement, EditAnywhere, BlueprintReadWrite)
        float AccelerationSeconds;
    float AccelerationLerpSeconds;

    // Health
    UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadOnly)
        class UHealthComponent* HealthComponent;

    // Shooting
    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        UClass* DefaultProjectile;

    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        FVector BulletSpawnOffset;

    UPROPERTY(Category = "Shooting", EditAnywhere, BlueprintReadWrite)
        float FireRate;

    bool bCanFire;
    FRotator CannonRotation;
    FTimerHandle FireCooldownTimerHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
    FORCEINLINE class UStaticMeshComponent* GetStaticMesh() const { return TankMainBodyMesh; }
    UFUNCTION(BlueprintCallable, Category = "Get/Set")
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    
    // Audio 
protected:
    UPROPERTY(Category = Audio, EditDefaultsOnly, BlueprintReadWrite)
        class USoundBase* FireSound;
};

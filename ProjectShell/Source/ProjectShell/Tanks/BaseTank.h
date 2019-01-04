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

    // Weapons
    void FireShot(FVector FireDirection);
    void ShotTimerExpired();

protected:
    static const FName kMoveForwardBinding;
    static const FName kMoveRightBinding;
    static const FName kFireForwardBinding;
    static const FName kFireRightBinding;

    UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UStaticMeshComponent* _tankStaticMesh;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
        FVector _bulletSpawnOffset;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
        float _fireRate;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
        float _moveSpeed;

    UPROPERTY(Category = Audio, EditDefaultsOnly, BlueprintReadWrite)
        class USoundBase* _fireSound;

    FTimerHandle _fireCooldownTimerHandle;

    bool _canFire;

public:
    /** Returns Static Mesh of tank **/
    FORCEINLINE class UStaticMeshComponent* GetStaticMesh() const { return _tankStaticMesh; }
    
};

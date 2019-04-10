// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectShellGameMode.generated.h"

USTRUCT(BlueprintType)
struct FGameModePlayerData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
        int PlayerControllerIndex;

    UPROPERTY(BlueprintReadWrite)
        APlayerStart* DesignatedPlayerStart;

    UPROPERTY(BlueprintReadWrite)
        int Lives;

    FGameModePlayerData()
    {
        PlayerControllerIndex = -1;
        DesignatedPlayerStart = NULL;
        Lives = 0;
    }
};

UCLASS(MinimalAPI)
class AProjectShellGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AProjectShellGameMode();

private:
    void StartPlay();

    UClass* PlayerTankBP;

protected:
    UWorld* World;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode Settings")
        int TankLives;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode Settings")
        float RespawnSeconds;

    UPROPERTY(BlueprintReadWrite)
        TArray<FGameModePlayerData> GameModePlayerData;

    FGameModePlayerData CreatePlayerDataForIndex(int index);

    UFUNCTION() // Needs to be UFUNCTION to be able to bind
        void TankDestroyedHandler(AActor* destroyedActor);
        
    void TankRespawn(FGameModePlayerData playerData);
};




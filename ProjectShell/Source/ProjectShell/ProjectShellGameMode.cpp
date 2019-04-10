// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectShellGameMode.h"
#include "Tanks/BaseTank.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h" // For debug on screen

AProjectShellGameMode::AProjectShellGameMode()
{
    static ConstructorHelpers::FObjectFinder<UClass> testTankBP(TEXT("Class'/Game/Blueprints/Tanks/TestPTank.TestPTank_C'"));
    if (testTankBP.Succeeded())
    {
        PlayerTankBP = testTankBP.Object;
    }

    // Set default pawn class to our tank blueprint
    DefaultPawnClass = PlayerTankBP;

    // Lives
    TankLives = 3;
    RespawnSeconds = 5;

    CreatePlayerDataForIndex(0); // Default player is spawned by UE4 - we fire this on construction anticipating that as StartPlay is too late
}

void AProjectShellGameMode::StartPlay()
{
    Super::StartPlay();

    World = GetWorld();

    if (World)
    {
        GameModePlayerData[0].Lives = TankLives; // Have to reset player 0 lives here as Blueprint initialization values aren't ready when player 0 data is created - TODO revisit logic
        CreatePlayerDataForIndex(1);
        UGameplayStatics::CreatePlayer(World, 1, true);

        for (int i = 0; i < GameModePlayerData.Num(); i++)
        {
            ABaseTank* playerTank = Cast<ABaseTank>(UGameplayStatics::GetPlayerPawn(World, i));
            if (playerTank)
            {
                playerTank->SetPlayerIndex(i);
                if (TankLives > 0)
                {
                    playerTank->OnDestroyed.AddDynamic(this, &AProjectShellGameMode::TankDestroyedHandler);
                }
            }
        }
    }
}

FGameModePlayerData AProjectShellGameMode::CreatePlayerDataForIndex(int index)
{
    FGameModePlayerData newPlayerData;
    newPlayerData.PlayerControllerIndex = index;
    newPlayerData.Lives = TankLives;

    GameModePlayerData.Add(newPlayerData);

    return newPlayerData;
}

void AProjectShellGameMode::TankDestroyedHandler(AActor* destroyedActor)
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("GAME MODE ACKNOWLEDGES DEATH"));
    ABaseTank* destroyedTank = Cast<ABaseTank>(destroyedActor);
    if (destroyedTank)
    {
        int playerIndex = destroyedTank->GetPlayerIndex();
        if (playerIndex >= 0 && playerIndex < GameModePlayerData.Num())
        {
            FTimerHandle tankRespawnTimerHandle;
            World->GetTimerManager().SetTimer(tankRespawnTimerHandle, FTimerDelegate::CreateUObject(this, &AProjectShellGameMode::TankRespawn, GameModePlayerData[playerIndex]), RespawnSeconds, false);
        }
    }
}

void AProjectShellGameMode::TankRespawn(FGameModePlayerData playerData)
{
    playerData.Lives--;

    APlayerController* playerController = UGameplayStatics::GetPlayerController(World, playerData.PlayerControllerIndex);
    if (playerData.Lives > 0 && playerController && playerData.DesignatedPlayerStart)
    {
        const FActorSpawnParameters spawnParams = FActorSpawnParameters();
        ABaseTank* newTank = World->SpawnActor<ABaseTank>(DefaultPawnClass, playerData.DesignatedPlayerStart->GetActorLocation(), playerData.DesignatedPlayerStart->GetActorRotation(), spawnParams);
        playerController->Possess(newTank);
        newTank->SetPlayerIndex(playerData.PlayerControllerIndex);
        newTank->OnDestroyed.AddDynamic(this, &AProjectShellGameMode::TankDestroyedHandler);
    }
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("GAME MODE ACKNOWLEDGES DEATH"));

    GameModePlayerData[playerData.PlayerControllerIndex] = playerData;
}
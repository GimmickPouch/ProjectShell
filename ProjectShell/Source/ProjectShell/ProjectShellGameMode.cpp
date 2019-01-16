// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectShellGameMode.h"
//#include "ProjectShellPawn.h"
#include "Tanks/BaseTank.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AProjectShellGameMode::AProjectShellGameMode()
{
    static ConstructorHelpers::FObjectFinder<UClass> testTankBP(TEXT("Class'/Game/Blueprints/Tanks/TestTank.TestTank_C'"));
    if (testTankBP.Succeeded())
    {
        _playerTestBP = testTankBP.Object;
    }

    // set default pawn class to our character class
    DefaultPawnClass = _playerTestBP; //ABaseTank::StaticClass(); //AProjectShellPawn::StaticClass();
}

void AProjectShellGameMode::StartPlay()
{
    Super::StartPlay();

    auto world = (UObject*)GetWorld();
    if (world != nullptr)
    {
        UGameplayStatics::CreatePlayer(world, 1, true);
        UGameplayStatics::CreatePlayer(world, 2, true);
    }
}

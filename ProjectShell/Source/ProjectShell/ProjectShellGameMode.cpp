// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectShellGameMode.h"
//#include "ProjectShellPawn.h"
#include "Tanks/BaseTank.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AProjectShellGameMode::AProjectShellGameMode()
{
<<<<<<< HEAD
    // TODO blueprint for new physics tank
    static ConstructorHelpers::FObjectFinder<UClass> testTankBP(TEXT("Class'/Game/Blueprints/Tanks/BasePhysicsTank.BasePhysicsTank_C'"));
=======
    static ConstructorHelpers::FObjectFinder<UClass> testTankBP(TEXT("Class'/Game/Blueprints/Tanks/TestTank.TestTank_C'"));
>>>>>>> parent of a7adc58... DRIFT TANKS
    if (testTankBP.Succeeded())
    {
        _playerTestBP = testTankBP.Object;
    }

    // set default pawn class to our character class
<<<<<<< HEAD
    DefaultPawnClass = _playerTestBP; //ABasePTank::StaticClass(); //_playerTestBP; //ABaseTank::StaticClass(); //AProjectShellPawn::StaticClass();
=======
    DefaultPawnClass = _playerTestBP; //ABaseTank::StaticClass(); //AProjectShellPawn::StaticClass();
>>>>>>> parent of a7adc58... DRIFT TANKS
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

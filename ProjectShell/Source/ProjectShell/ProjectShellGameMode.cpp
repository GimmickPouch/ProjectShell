// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectShellGameMode.h"
//#include "ProjectShellPawn.h"
#include "Tanks/BaseTank.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AProjectShellGameMode::AProjectShellGameMode()
{
    static ConstructorHelpers::FObjectFinder<UClass> testTankBP(TEXT("Class'/Game/Blueprints/Tanks/TestPTank.TestPTank_C'"));
    if (testTankBP.Succeeded())
    {
        PlayerTankBP = testTankBP.Object;
    }

    // Set default pawn class to our tank blueprint
    DefaultPawnClass = PlayerTankBP;
}

void AProjectShellGameMode::StartPlay()
{
    Super::StartPlay();

    auto world = (UObject*)GetWorld();
    if (world != nullptr)
    {
        UGameplayStatics::CreatePlayer(world, 1, true);
    }
}

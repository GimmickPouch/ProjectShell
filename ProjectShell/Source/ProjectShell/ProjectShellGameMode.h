// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectShellGameMode.generated.h"

UCLASS(MinimalAPI)
class AProjectShellGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AProjectShellGameMode();

private:
    UClass* _playerTestBP;
};




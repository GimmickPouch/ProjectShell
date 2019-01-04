// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectShellGameMode.h"
#include "ProjectShellPawn.h"

AProjectShellGameMode::AProjectShellGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AProjectShellPawn::StaticClass();
}


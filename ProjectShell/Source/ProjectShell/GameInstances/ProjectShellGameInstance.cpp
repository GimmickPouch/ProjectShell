// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectShellGameInstance.h"
#include "Singletons/BaseSingleton.h"

TArray<UBaseSingleton*> UProjectShellGameInstance::Singletons;

UProjectShellGameInstance::UProjectShellGameInstance()
{
    // Managers we want to spawn on game session start
    SingletonsToSpawn = 
    {
        UCameraManager::StaticClass()
    };
}

void UProjectShellGameInstance::OnStart()
{
    Super::OnStart();

    Singletons.Empty(); // Because during PIE static variables are maintained across sessions.
    for (TSubclassOf<UBaseSingleton> singletonType : SingletonsToSpawn)
    {
        RegisterSingleton(NewObject<UBaseSingleton>(this, singletonType));
    }
}

void UProjectShellGameInstance::RegisterSingleton(UBaseSingleton* singleton)
{
    Singletons.Add(singleton);
}

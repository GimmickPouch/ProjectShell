// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Singletons/Managers/CameraManager.h"
#include "ProjectShellGameInstance.generated.h"

UCLASS()
class PROJECTSHELL_API UProjectShellGameInstance : public UGameInstance
{
    GENERATED_BODY()
    
public:
    UProjectShellGameInstance();
    
protected:
    virtual void OnStart() override;
    
private:
    static TArray<UBaseSingleton*> Singletons;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Singletons")
        TArray<TSubclassOf<UBaseSingleton>> SingletonsToSpawn;

public:
    static void RegisterSingleton(UBaseSingleton* singleton);

    template <class T>
    FORCEINLINE static T* GetSingleton()
    {
        static_assert(std::is_base_of<UBaseSingleton, T>::value, "T must inherit from UBaseSingleton");
        T* singleton = NULL;
        if (Singletons.Num() > 0)
        {
            for (int i = 0; i < Singletons.Num(); i++)
            {
                if (Singletons[i] != nullptr && Singletons[i]->IsA(T::StaticClass()))
                {
                    singleton = (T*)Singletons[i];
                    break;
                }
            }
        }
        return singleton;
    }
};

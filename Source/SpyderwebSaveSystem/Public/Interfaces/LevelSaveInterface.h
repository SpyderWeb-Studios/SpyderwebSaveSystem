// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "UObject/Interface.h"
#include "LevelSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULevelSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPYDERWEBSAVESYSTEM_API ILevelSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System|Level")
	void UpdateActor(bool Interacted);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System|Level")
	void OnActorDataLoaded();
};

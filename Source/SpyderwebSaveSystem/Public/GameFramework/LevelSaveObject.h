// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Libraries/FDynamicLevelSerialiser.h"
#include "LevelSaveObject.generated.h"

/**
 * 
 */
UCLASS()
class SPYDERWEBSAVESYSTEM_API ULevelSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TMap<TObjectPtr<AActor>, bool> InteractedWithActors;
	
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TMap<TObjectPtr<AActor>, FTransform> MovedActors;

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void AddDynamicActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void RemoveDynamicActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void LoadDynamicActors(const UObject* WorldContextObject);
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TArray<FDynamicLevelSerialiser> DynamicActors;
	
};

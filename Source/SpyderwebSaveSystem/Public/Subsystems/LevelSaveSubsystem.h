// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/LevelSaveObject.h"
#include "GameFramework/SaveGame.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelSaveSubsystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SPYDERWEBSAVESYSTEM_API ULevelSaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	ULevelSaveSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Save System|Level", meta=(DeprecatedFunction, DeprecationMessage = "Use UpdateDynamicActors instead"))
	virtual void UpdateActors(AActor* SavedActor, bool bInteracted);

	UFUNCTION(BlueprintCallable, Category = "Save System|Level", meta=(DeprecatedFunction, DeprecationMessage = "Use UpdateDynamicActors instead"))
	virtual void UpdateMovedActors(AActor* SavedActor, FTransform Transform);

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void UpdateDynamicActors(AActor* SavedActor);

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void RemoveDynamicActor(AActor* SavedActor);
	
	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void SaveData();

	UFUNCTION(BlueprintCallable, Category = "Save System|Level")
	void LoadData();
	
protected:

	UFUNCTION()
	virtual void OnAsyncLoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
		
	UFUNCTION()
	virtual void OnAsyncSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	FString LevelSaveSlot = "LevelSlot";
	
private:


	UPROPERTY()
	TObjectPtr<ULevelSaveObject> LevelSaveObject;
	
	
};

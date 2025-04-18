// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/LevelSaveSubsystem.h"
#include "Interfaces/LevelSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SpyderwebSaveSystem.h"
#include "Libraries/SaveSystemMacros.h"

ULevelSaveSubsystem::ULevelSaveSubsystem()
{
}

void ULevelSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LevelSaveSlot = GetWorld()->GetFName().ToString();

	UE_LOG(LogSaveSystem, Display, TEXT("Level Save Slot: %s"), *LevelSaveSlot);

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ULevelSaveSubsystem::LoadData);
}


void ULevelSaveSubsystem::UpdateActors(AActor* SavedActor, bool bInteracted)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Updating  Level Save Data"));
	if(LevelSaveObject && IsValid(SavedActor))
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Save Object and Actor are Valid"));
		LevelSaveObject->InteractedWithActors.Add(SavedActor, bInteracted);
	}
}

void ULevelSaveSubsystem::UpdateMovedActors(AActor* SavedActor, FTransform Transform)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Updating Level  Save Data (Moved Actor)"));
	if(LevelSaveObject && SavedActor)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Save Object and Actor are Valid"));
		LevelSaveObject->MovedActors.Add(SavedActor, Transform);
	}
}

void ULevelSaveSubsystem::UpdateDynamicActors(AActor* SavedActor)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Updating Level  Save Data (Dynamic Actor)"));
	if(LevelSaveObject && SavedActor)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Save Object and Actor are Valid"));
		LevelSaveObject->AddDynamicActor(SavedActor);
	}
}

void ULevelSaveSubsystem::RemoveDynamicActor(AActor* SavedActor)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Removing Dynamic Actor from Level Save Data"));
	if(LevelSaveObject && SavedActor)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Save Object and Actor are Valid"));
		LevelSaveObject->RemoveDynamicActor(SavedActor);
	}
}

void ULevelSaveSubsystem::OnAsyncLoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Level Async Loading Finished"));
	if(SaveGame)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Level Save Game Pointer is Valid"));
		LevelSaveObject = Cast<ULevelSaveObject>(SaveGame);

		// Use the Save Data to Affect which Actors have been interacted with
		for(auto SavedActor : LevelSaveObject->InteractedWithActors)
		{
			UE_LOG(LogSaveSystem, Display, TEXT("Attempting to Update Actor"));
			if(IsValid(SavedActor.Key) && SavedActor.Key->Implements<ULevelSaveInterface>())
			{
				ILevelSaveInterface::Execute_UpdateActor(SavedActor.Key, SavedActor.Value);
			}
		}

		LevelSaveObject->LoadDynamicActors(this);
	}
}

void ULevelSaveSubsystem::OnAsyncSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	UE_LOG(LogSaveSystem, Display, TEXT("Level Async Saving Finished"));
	if(bSuccess)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Level Save was Successful"));
	}
}

void ULevelSaveSubsystem::SaveData()
{
#if ENABLE_LEVEL_SAVE_SYSTEM
	UE_LOG(LogSaveSystem, Display, TEXT( "Saving Level Data"));
	// If it isn't Valid, Create a new instance
	if(!IsValid(LevelSaveObject))
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Player Save is NOT Valid. Creating New Instance"));
		LevelSaveObject = Cast<ULevelSaveObject>(UGameplayStatics::CreateSaveGameObject(ULevelSaveObject::StaticClass()));
	}
	FAsyncSaveGameToSlotDelegate asyncSaveDelegate;
	asyncSaveDelegate.BindUObject(this, &ULevelSaveSubsystem::OnAsyncSaveFinished);
	UGameplayStatics::AsyncSaveGameToSlot(LevelSaveObject, LevelSaveSlot, 0, asyncSaveDelegate);
#endif
}

void ULevelSaveSubsystem::LoadData()
{
#if ENABLE_LEVEL_SAVE_SYSTEM
	UE_LOG(LogSaveSystem, Display, TEXT( "Attempting to Load Level Data"));

	// If a save game exists in a slot, then load it
	if(UGameplayStatics::DoesSaveGameExist(LevelSaveSlot, 0))
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Level Save Data Exists. Async Loading"));
		
		FAsyncLoadGameFromSlotDelegate asyncLoadDelegate;
		asyncLoadDelegate.BindUObject(this, &ULevelSaveSubsystem::OnAsyncLoadFinished);
		UGameplayStatics::AsyncLoadGameFromSlot(LevelSaveSlot, 0, asyncLoadDelegate);
	}

	// Otherwise, create one
	else
	{
		UE_LOG(LogSaveSystem, Display, TEXT("No Player Save Data Exists. Creating New One"));
		OnAsyncLoadFinished(LevelSaveSlot, 0, UGameplayStatics::CreateSaveGameObject(ULevelSaveObject::StaticClass()));
	}
#endif
}

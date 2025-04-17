// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/LevelSaveObject.h"

#include "SpyderwebSaveSystem.h"

void ULevelSaveObject::AddDynamicActor(AActor* Actor)
{
	FDynamicLevelSerialiser Serialiser;
	Serialiser.SerialiseActor(Actor);
	const bool bSuccess = DynamicActors.IsValidIndex(DynamicActors.AddUnique(Serialiser));
	UE_LOG(LogSaveSystem, Display, TEXT("Actor [%s] was %s added to the dynamic actors list"), *GetNameSafe(Actor), bSuccess ? TEXT("") : TEXT("not"));
}

void ULevelSaveObject::RemoveDynamicActor(AActor* Actor)
{
	DynamicActors.RemoveAll([Actor](const FDynamicLevelSerialiser& Serialiser)
	{
		return Serialiser.Actor == Actor;
	});
}

void ULevelSaveObject::LoadDynamicActors(const UObject* WorldContextObject)
{
	for (FDynamicLevelSerialiser& Serialiser : DynamicActors)
	{
		Serialiser.DeserialiseActor(WorldContextObject);
	}
}

#include "Libraries/FDynamicLevelSerialiser.h"

#include "SpyderwebSaveSystem.h"

void FDynamicLevelSerialiser::SerialiseActor(AActor* InActor)
{
	if(!IsValid(InActor))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Serialisation failed: Actor is not valid"));
		return;
	}

	UE_LOG(LogSaveSystem, Display, TEXT("Serialising actor: %s"), *GetNameSafe(InActor));
	Transform = InActor->GetActorTransform();
	Actor = InActor;
	ActorClass = InActor->GetClass();


	// Serialise the actor's data
	FMemoryWriter MemoryWriter(ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
	InActor->Serialize(Ar);
	
}

void FDynamicLevelSerialiser::DeserialiseActor(const UObject* WorldContextObject)
{
	if(Actor.IsValid() && Actor->HasAnyFlags(RF_WasLoaded))
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Actor [%s] was loaded)"), *GetNameSafe(Actor.Get()));
		if(!IsValid(Actor.Get()))
		{
			UE_LOG(LogSaveSystem, Warning, TEXT("Deserialisation failed: Actor is not valid"));
			return;
		}
	}
	else
	{
		checkf(WorldContextObject, TEXT("WorldContextObject is not valid, cannot deserialise actor"));
		if(Actor.IsValid())
		{
			UE_LOG(LogSaveSystem, Warning, TEXT("Deserialisation failed: Actor [%s] is already loaded"), *GetNameSafe(Actor.Get()));
			return;
		}
		// Spawn in a new Instance of the Actor
		UE_LOG(LogSaveSystem, Display, TEXT("Actor [%s] was not loaded, spawning new instance"), *GetNameSafe(Actor.Get()));
		Actor = WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass.LoadSynchronous(), Transform);
	}
	
	if(!IsValid(Actor.Get()))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Deserialisation failed: Actor is not valid"));
		return;
	}
	
	PerformDeserialisation();
}

void FDynamicLevelSerialiser::PerformDeserialisation() const
{
	UE_LOG(LogSaveSystem, Display, TEXT("Deserialising actor: %s"), *GetNameSafe(Actor.Get()));
	Actor.Get()->SetActorTransform(Transform);
 
	// Deserialise the actor's data
	FMemoryReader MemoryReader(ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
	Actor.Get()->Serialize(Ar);

	if(Actor.Get()->Implements<ULevelSaveInterface>())
	{
		ILevelSaveInterface::Execute_OnActorDataLoaded(Actor.Get());
	}
	else
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Actor %s does not implement ILevelSaveInterface"), *GetNameSafe(Actor.Get()));
	}
}

bool FDynamicLevelSerialiser::operator==(const FDynamicLevelSerialiser& Other) const
{
	return Actor == Other.Actor;
}


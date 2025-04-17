#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Interfaces/LevelSaveInterface.h"
#include "FDynamicLevelSerialiser.generated.h"


// This Serialiser is used to save and load the level data for a single actor that changes during gameplay,
// mainly actors that have been spawned or destroyed, or actors that have been moved.
USTRUCT(BlueprintType)
struct FDynamicLevelSerialiser
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TSoftObjectPtr<AActor> Actor;

	/* The class of the Actor, only required for placed actors */
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TSoftClassPtr<AActor> ActorClass;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	FTransform Transform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(BlueprintReadOnly, Category = "Save System|Level")
	TArray<uint8> ByteData;
	
	void SerialiseActor(AActor* InActor);

	void DeserialiseActor(const UObject* WorldContextObject);

	bool operator==(const FDynamicLevelSerialiser& Other) const;
protected:

	void PerformDeserialisation() const;

};
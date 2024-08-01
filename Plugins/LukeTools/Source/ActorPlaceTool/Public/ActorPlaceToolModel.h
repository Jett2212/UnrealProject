// Luke Jackson 29/07/2024
#pragma once

#include "CoreMinimal.h"
#include "ActorPlaceToolView.h"
#include "Components/PrimitiveComponent.h"

class FAssetRegistryModule;

/////////////////////////

// Seperate this into nother file. Create a FComponentVisualiser and use these to communicate and draw location on a toggle
// Future todo: Use visualisert to create bounds for multi placement and randomisation;
// Seperate bounds by enum to different types e.g. cube, sphere ect. 
// Uniform placement and random placement in zone / at radius from point?
class URenderComp : public UPrimitiveComponent
{
public:
	URenderComp() {};

	void TestFunction() { UE_LOG(LogTemp, Warning, TEXT("Component Works")); }
};

class ARenderActor : public AActor
{
public:
	ARenderActor();

	URenderComp* RenderComp{ nullptr };
};


////////////////////
class ActorPlaceToolModel
{
public:
	ActorPlaceToolModel();

	void Init(SActorPlaceToolView* InView);
	void DeInit();
	void IsFinishedLoading();

	void ObjectChanged(const FAssetData& InAssetData);
	FString GetObjectPath();

	void TestButtonPressed();
	void TestVisualiser();
	void TestSpawnActor();

	inline void SetPlaceVector(const FVector& InVector) { PlaceVector = InVector; }
	void SetPlaceVectorFloat(TOptional<float> InFloat, EAxis::Type Axis);

	TOptional<float> GetPlaceVectorFloat(EAxis::Type Axis) const;
	bool bAssetRegistryLoaded{ true };
	
	SActorPlaceToolView* ToolView{ nullptr };
	
private:
	FVector PlaceVector{ FVector::ZeroVector };
	FRotator PlaceRotation{ FRotator::ZeroRotator };
	FVector PlacementScale{ FVector(1.f, 1.f, 1.f)};

	FAssetRegistryModule* AssetRegistryModule;

	UObject* ObjectToPlace{ nullptr };
	FString ObjectPath{ TEXT("") };

	ARenderActor* RenderActor{ nullptr };
};


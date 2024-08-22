// Luke Jackson 29/07/2024
#pragma once

#include "CoreMinimal.h"
#include "ActorPlaceToolView.h"


class ARenderActor;
class FAssetRegistryModule;

UENUM()
enum EMinMax : uint8
{
	Min, Max
};

UENUM()
enum EPlacementMethod : uint8
{
	UniformPlacement, RandomPathing, RandomLineSim, RandomChaos
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

	void SpawnSingleActor();

	void SpawnActor(const FVector& InVector, const FRotator& InRotation);

	void BoxWidthChanged(float InBoxWidth);
	float GetBoxWidth() { return BoxWidth; }

	// Box
	void BoxXChanged(double InBoxX);
	double GetBoxX() const { return BoxX; } 
	void BoxYChanged(double InBoxY);
	double GetBoxY() const { return BoxY; } 

	void BoxTypeChanged();
	// Uniform 
	void UpdateNonUniformBox();

	void UniformSpawnDistanceChanged(double InUniformSpawnDistance);
	float GetUniformSpawnDistance() { return UniformSpawnDistance; }
	void UniformMethod();

	//Random
	void RandomDistanceChanged(const double& InDouble, EMinMax MinMaxType);
	double GetRandomDistance(EMinMax MinMaxType);
	void UpdateActorPlacementArray();
	bool GenerateLocationRecursive(const int32& InSeed, const FVector& InVector, FVector& OutVector, double DistanceToPlace, const TArray<FVector>& VectorsToTest, const int32& MaxCount, int32& CurrentCount);

	inline void SetPlaceVector(const FVector& InVector) { PlaceVector = InVector; }
	void SetPlaceVectorFloat(TOptional<float> InFloat, EAxis::Type Axis);

	TOptional<float> GetPlaceVectorFloat(EAxis::Type Axis) const;

	
	void SetRotationFloat(float InFloat, EAxis::Type Axis);
	TOptional<float> GetRotationFloat(EAxis::Type Axis) const;

	bool bAssetRegistryLoaded{ true };
	
	SActorPlaceToolView* ToolView{ nullptr };

	bool bUniformBoxSize{ false };

	bool bRandomPlacement{ false };

	EPlacementMethod PlacementOption { EPlacementMethod::UniformPlacement };

	void RandomCountChanged(const int32& InCount);
	int32 GetRandomCount() const { return RandomCount; }

	void RandomiseSeed();

	void SpawnActors();


	void SetSeed(const int32& InInt);
	int32 GetSeed() const { return Seed; }


	void SetLineCount(const int32& InLineCount);
	int32 GetLineCount() const { return LineCount; }


	bool GetShouldRaycast() const { return bShouldRaycast ;}
	void SetShouldRaycast( const bool bInShouldRaycast );

	void SelectRenderActor();

	
private:
	FVector PlaceVector{ FVector::ZeroVector };
	FRotator PlaceRotation{ FRotator::ZeroRotator };
	FVector PlacementScale{ FVector(1.0, 1.0, 1.0)};

	FAssetRegistryModule* AssetRegistryModule;

	UObject* ObjectToPlace{ nullptr };
	FString ObjectPath{ TEXT("") };

	ARenderActor* RenderActor{ nullptr };

	int32 Seed {0};
	int32 LineCount {1};
	bool bShouldRaycast { false };

	//Box
	float BoxWidth;
	double BoxX{ 0.0 };
	double BoxY{ 0.0 };
	double DistanceFromEdge{ 0.0 };
	bool bDrawBoxForMultiPlace{ true };

	//Uniform
	double UniformSpawnDistance{ 100.0 };

	//Random
	double MinDistance {50.0};
	double MaxDistance {100.0};

	int32 RandomCount {0};

	void RandomMethodPathing();
	void RandomMethodLineSim();
	void RandomMethodChaos();

	TArray<FVector> ActorPlacementArray;

	void RaycastAllArray();
	void RenderActorMoved(FVector InVec, FRotator InRot);

	void UpdateRenderActorArray(const TArray<FVector>& Vectors, bool bShouldCheckDistance);
};


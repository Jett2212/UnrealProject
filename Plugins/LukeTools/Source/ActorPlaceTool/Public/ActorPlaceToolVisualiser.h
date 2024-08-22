// Luke Jackson 03/08/2024

#pragma once

#include "DebugRenderSceneProxy.h"
#include "Components/PrimitiveComponent.h"
#include "Debug/DebugDrawComponent.h"

#include "ActorPlaceToolVisualiser.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FRenderActorMoved, FVector, FRotator)

class FActorPlaceToolSceneProxy : public FDebugRenderSceneProxy
{
public: 
	FActorPlaceToolSceneProxy( const UPrimitiveComponent* InComponent);

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	//GetDynamicMeshElementsForView // Override this for custom shapes drawing

};

/////////////////////////

// Seperate this into nother file. Create a FComponentVisualiser and use these to communicate and draw location on a toggle
// Future todo: Use visualisert to create bounds for multi placement and randomisation;
// Seperate bounds by enum to different types e.g. cube, sphere ect. 
// Uniform placement and random placement in zone / at radius from point?
UCLASS()
class UActorPlaceToolDebugDrawComponent : public UDebugDrawComponent
{
	GENERATED_BODY()
public:
	UActorPlaceToolDebugDrawComponent();

	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	virtual bool ShouldRecreateProxyOnUpdateTransform() const override { return true; }

	void UpdateSceneProxy();
	//void TestFunc() { UE_LOG(LogTemp, Warning, TEXT("THis Works At least")); }
	//void TestFunction() { UE_LOG(LogTemp, Warning, TEXT("Component Works")); }
};

UCLASS()
class ARenderActor : public AActor
{
	GENERATED_BODY()
public:
	ARenderActor();
	void DestroySelf();

	struct Sphere
	{
		float Radius{ 100.f };
		FVector LocationToDraw{ FVector::ZeroVector };
		FColor Colour{ FColor::Blue };

		Sphere() {}
		Sphere(float InRadius, FVector InLocationToDraw, FColor InColour)
			:Radius(InRadius), LocationToDraw(InLocationToDraw), Colour(InColour)
		{}
	};

	Sphere MainPoint{ };

	TArray<Sphere> SpheresToDraw;

	struct Box
	{
		FVector FirstPoint{ FVector::ZeroVector };
		FVector SecondPoint{ FVector::ZeroVector };
		FColor Colour{ FColor::Blue };

		Box() {};

		Box(FVector InFirstPoint, FVector InSecondPoint, FColor InColour)
			:FirstPoint(InFirstPoint), SecondPoint(InSecondPoint), Colour(InColour)
		{}
		Box(float Width, FVector CenterPoint, FColor InColour)
		{
			FirstPoint = CenterPoint - (Width / 2);
			SecondPoint = CenterPoint + (Width / 2);
			Colour = InColour;
		}
		Box(const double& InX, const double& InY, const FVector& CenterPoint, const FColor& InColour)
		{
			FirstPoint.X = CenterPoint.X - (InX / 2);
			FirstPoint.Y = CenterPoint.Y - (InY / 2);
			FirstPoint.Z = CenterPoint.Z - 100.f;

			SecondPoint.X = CenterPoint.X + (InX / 2);
			SecondPoint.Y = CenterPoint.Y + (InY / 2);
			SecondPoint.Z = CenterPoint.Z + 100.f;

			Colour = InColour;
		}
	};
	Box BoxToDraw{ Box() };

	UActorPlaceToolDebugDrawComponent* ActorPlaceToolDebugDrawComponent{ nullptr };

	void SetupComponent();

	void AddToArray(const FVector& InVec);

	void UpdateMainPoint(const FVector& InVec);

	void SetBoxWidth(const double& InWidth);
	void SetBoxXY(const double& InX, const double& InY);

	double BoxX{ 100.f };
	double BoxY{ 100.f };

	bool bShouldPreviewActor{ true };
	void UpdateActorPreview(const TArray<FVector>& InArray);
	TArray<FVector> ActorPlacementArray;

	float BoxWidth{ 0.f };

	void UpdateSceneProxy();
	//URenderComp* RenderComp{ nullptr };

	FRenderActorMoved& GetRenderActorMovedDelegate() { return RenderActorMovedDelegate; }

protected:
	virtual void PostEditMove(bool bFinished) override;

private:
	void CreateBoxWithExistingXY();

	FRenderActorMoved RenderActorMovedDelegate;
};

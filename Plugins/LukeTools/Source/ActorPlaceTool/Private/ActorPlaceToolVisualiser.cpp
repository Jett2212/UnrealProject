#include "ActorPlaceToolVisualiser.h"
#include "Components/SceneComponent.h"


FActorPlaceToolSceneProxy::FActorPlaceToolSceneProxy(const UPrimitiveComponent* InComponent)
	:FDebugRenderSceneProxy(InComponent)
{
	DrawType = EDrawType::WireMesh;
	DrawAlpha = 1;
}

FPrimitiveViewRelevance FActorPlaceToolSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance ViewRelevance;

	ViewRelevance.bDrawRelevance = IsShown(View) && ViewFlagIndex != INDEX_NONE;

	ViewRelevance.bSeparateTranslucency = ViewRelevance.bNormalTranslucency = IsShown(View);
	ViewRelevance.bDynamicRelevance = true;
	ViewRelevance.bShadowRelevance = IsShadowCast(View);

	return ViewRelevance;
}

UActorPlaceToolDebugDrawComponent::UActorPlaceToolDebugDrawComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCastShadow(false);
	SetHiddenInGame(true);
	bVisibleInReflectionCaptures = false;
	bVisibleInRayTracing = false;
	bVisibleInRealTimeSkyCaptures = false;
	// Exclude this component from non-editor builds
	bIsEditorOnly = true;

	SetIsVisualizationComponent(true);
}

FDebugRenderSceneProxy* UActorPlaceToolDebugDrawComponent::CreateDebugSceneProxy()
{
	FActorPlaceToolSceneProxy* ActorPlaceToolSceneProxy =  new FActorPlaceToolSceneProxy(this);

	
	

	if (ARenderActor* Owner = GetOwner<ARenderActor>())
	{
		// Main Point
		ActorPlaceToolSceneProxy->Spheres.Add(FDebugRenderSceneProxy::FSphere(Owner->MainPoint.Radius, Owner->MainPoint.LocationToDraw, Owner->MainPoint.Colour));

		// Spheres
		for (ARenderActor::Sphere SphereToDraw : Owner->SpheresToDraw)
		{
			ActorPlaceToolSceneProxy->Spheres.Add(FDebugRenderSceneProxy::FSphere(SphereToDraw.Radius, SphereToDraw.LocationToDraw, SphereToDraw.Colour));
		}

		// Box
		//TArray<FVector> BoxPts{ FVector(0, 0, 200), FVector(100, 400, 500)};
		ActorPlaceToolSceneProxy->Boxes.Empty();
		FTransform TempTransform = FTransform();
		//TempTransform.SetLocation(Owner->GetActorRotation().RotateVector(Owner->GetActorLocation()) );
		//TempTransform.SetLocation(FVector(0.0));
		//TempTransform.SetRotation(Owner->GetActorRotation().Quaternion());
		//TempTransform.SetRotation(FQuat::MakeFromEuler(FVector(0.1, 0.2, 1.2)));
		//ActorPlaceToolSceneProxy->Boxes.Add(FDebugRenderSceneProxy::FDebugBox(FBox(Owner->GetActorRotation().RotateVector(Owner->BoxToDraw.FirstPoint), OwnerGetActorRotation().RotateVector(Owner->BoxToDraw.SecondPoint)), Owner->BoxToDraw.Colour, TempTransform));
		ActorPlaceToolSceneProxy->Boxes.Add(FDebugRenderSceneProxy::FDebugBox(FBox(Owner->BoxToDraw.FirstPoint, Owner->BoxToDraw.SecondPoint), Owner->BoxToDraw.Colour, Owner->GetTransform()));

		//Owner->GetTransform().

		if (Owner->bShouldPreviewActor)
		{
			for (const FVector& Vec : Owner->ActorPlacementArray)
			{
				//FMatrix TempMatrix = FMatrix();
				//TempMatrix.SetOrigin(Vec);
				//ActorPlaceToolSceneProxy->Cones.Add(FDebugRenderSceneProxy::FCone(TempMatrix, 30.f, 30.f, FColor::Green));
				ActorPlaceToolSceneProxy->Spheres.Add(FDebugRenderSceneProxy::FSphere(20.f, Vec, FColor::Green));
			}
		}
		//Line 
		
		//ActorPlaceToolSceneProxy->Lines.Add(FDebugRenderSceneProxy::FDebugLine())
		
		//ActorPlaceToolSceneProxy->Cones.Add(FDebugRenderSceneProxy::FCone(FMatrix(FVector(0), )
	}
	return ActorPlaceToolSceneProxy;
}

FBoxSphereBounds UActorPlaceToolDebugDrawComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds::Builder BoundsBuilder;
	BoundsBuilder += Super::CalcBounds(LocalToWorld);

	BoundsBuilder += FSphere(GetComponentLocation(), 500.f);

	if (ARenderActor* Owner = GetOwner<ARenderActor>())
	{
		for (const ARenderActor::Sphere SphereToDraw : Owner->SpheresToDraw)
		{
			BoundsBuilder += SphereToDraw.LocationToDraw;
		}
		BoundsBuilder += Owner->BoxToDraw.FirstPoint;
		BoundsBuilder += Owner->BoxToDraw.SecondPoint;
	}
	
	return BoundsBuilder;
}

void UActorPlaceToolDebugDrawComponent::UpdateSceneProxy()
{
	MarkRenderStateDirty();

}


ARenderActor::ARenderActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("RootComponent")));
	//ActorPlaceToolDebugDrawComponent->RegisterComponent();
	//UE_LOG(LogTemp, Warning, TEXT("Created"));
	//RenderComp = CreateDefaultSubobject<URenderComp>(FName(TEXT("RenderComponent")));
}

void ARenderActor::DestroySelf ()
{
	ActorPlacementArray.Empty();
	ActorPlaceToolDebugDrawComponent = nullptr;

	SpheresToDraw.Empty();
	
	this->Destroy();
}

void ARenderActor::SetupComponent()
{
	if (!ActorPlaceToolDebugDrawComponent)
	{
		ActorPlaceToolDebugDrawComponent = NewObject<UActorPlaceToolDebugDrawComponent>(this);
		ActorPlaceToolDebugDrawComponent->RegisterComponent();
		
		AddInstanceComponent(ActorPlaceToolDebugDrawComponent);
		//AddComponentByClass(UActorPlaceToolDebugDrawComponent::StaticClass(), false, FTransform(), false);
		//ActorPlaceToolDebugDrawComponent = <UActorPlaceToolDebugDrawComponent>();
	}
}

void ARenderActor::AddToArray(const FVector& InVec)
{
	if (ActorPlaceToolDebugDrawComponent)
	{
		//ActorPlaceToolDebugDrawComponent->TestFunc();
		//ActorPlaceToolDebugDrawComponent->TestVectorsToDraw.Emplace(InVec);
		//TestVectorsToDraw.Add(InVec);


		SpheresToDraw.Add(Sphere(100.f, InVec, FColor::Blue));
		//BoxToDraw = Box(300, InVec, FColor::Red);
		ActorPlaceToolDebugDrawComponent->UpdateSceneProxy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DOesNotExist"));
	}

}
void ARenderActor::UpdateMainPoint(const FVector& InVec)
{
	if (ActorPlaceToolDebugDrawComponent)
	{
		MainPoint.LocationToDraw = InVec;
		CreateBoxWithExistingXY();
	}
}

void ARenderActor::UpdateSceneProxy()
{
	if (ActorPlaceToolDebugDrawComponent)
	{
		ActorPlaceToolDebugDrawComponent->UpdateSceneProxy();
	}
}

void ARenderActor::SetBoxWidth(const double& InWidth)
{
	BoxWidth = InWidth;
	BoxX = InWidth;
	BoxY = InWidth;
	CreateBoxWithExistingXY();
}

void ARenderActor::SetBoxXY(const double& InX, const double& InY)
{

	BoxX = InX;
	BoxY = InY;
	CreateBoxWithExistingXY();
}

void ARenderActor::PostEditMove ( bool bFinished )
{
	RenderActorMovedDelegate.Broadcast(GetActorLocation(), GetActorRotation());
}

void ARenderActor::CreateBoxWithExistingXY()
{
	//BoxToDraw = Box(BoxX, BoxY, MainPoint.LocationToDraw, FColor::Red);
	BoxToDraw = Box(BoxX, BoxY, FVector(0.0), FColor::Red);
	UpdateSceneProxy();
}

void ARenderActor::UpdateActorPreview(const TArray<FVector>& InArray)
{
	ActorPlacementArray = InArray;
	UpdateSceneProxy();
}
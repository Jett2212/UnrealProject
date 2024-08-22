// Luke Jackson 29/07/2024

#include "ActorPlaceToolModel.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ActorPlaceToolVisualiser.h"
#include "RandomisationStatics.h"
#include "Async/ParallelFor.h"
#include "Logging/StructuredLog.h"
#include "Selection.h"

ActorPlaceToolModel::ActorPlaceToolModel()
{
	//CreateDefaultSubobject()
	//RenderComp = NewObject<URenderComp>();
	
}

void ActorPlaceToolModel::Init(SActorPlaceToolView* InView)
{
	ToolView = InView;
	//RenderActor = NewObject<ARenderActor>();
	//AActor()
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.ObjectFlags = RF_Transient;
		RenderActor = World->SpawnActor< ARenderActor>(ARenderActor::StaticClass(), SpawnParam);
		
	}
	if (RenderActor)
	{
		
		RenderActor->SetupComponent();
		RenderActor->GetRenderActorMovedDelegate().AddRaw(this, &ActorPlaceToolModel::RenderActorMoved);
	}
	if (ToolView)
	{
		FAssetRegistryModule& RAssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		AssetRegistryModule = &RAssetRegistryModule;
		ToolView->InitView();
	}

}

void ActorPlaceToolModel::DeInit()
{
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		if (RenderActor)
		{
			RenderActor->DestroySelf();
			RenderActor->GetRenderActorMovedDelegate().RemoveAll(this);
//			RenderActor->ConditionalBeginDestroy();
//			RenderActor = nullptr;

			//World->DestroyActor(RenderActor);
			//RenderActor->Destroy();
		}
	}
	ToolView = nullptr;
	
	RenderActor = nullptr;
}

void ActorPlaceToolModel::IsFinishedLoading()
{
	if (AssetRegistryModule)
	{
		bAssetRegistryLoaded = !AssetRegistryModule->Get().IsLoadingAssets();

		ToolView->InitView();
	}
	//bAssetRegistryLoaded = !AssetRegistryModule.Get().IsLoadingAssets();
	//return true;
}

void ActorPlaceToolModel::ObjectChanged(const FAssetData& InAssetData)
{
	ObjectToPlace = InAssetData.GetAsset();
	if (IsValid(ObjectToPlace))
	{
		ObjectPath = InAssetData.GetObjectPathString();
	}
}

FString ActorPlaceToolModel::GetObjectPath()
{
	if (IsValid(ObjectToPlace))
	{
		return ObjectToPlace->GetPathName();
	}
	return TEXT("");

}

void ActorPlaceToolModel::SetPlaceVectorFloat(TOptional<float> InFloat, EAxis::Type Axis)
{
	switch (Axis)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		PlaceVector.X = InFloat.Get(0.f);
		break;
	case EAxis::Y:
		PlaceVector.Y = InFloat.Get(0.f);
		break;
	case EAxis::Z:
		PlaceVector.Z = InFloat.Get(0.f);
		break;
	default:
		break;
	}

	if (IsValid(RenderActor))
	{
		RenderActor->UpdateMainPoint(PlaceVector);
		
		RenderActor->SetActorLocation(PlaceVector);
		UpdateActorPlacementArray();
		if (GEditor->GetSelectedActors()->IsSelected(RenderActor))
		{
			GEditor->SelectNone(true, true);
		}

	}
}

TOptional<float> ActorPlaceToolModel::GetPlaceVectorFloat(EAxis::Type Axis) const
{
	float RetVal{ 0 };
	switch (Axis)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		RetVal = PlaceVector.X;
		break;
	case EAxis::Y:
		RetVal = PlaceVector.Y;
		break;
	case EAxis::Z:
		RetVal = PlaceVector.Z;
		break;
	default:
		break;
	}
	return RetVal;
}

void ActorPlaceToolModel::SetRotationFloat ( float InFloat, EAxis::Type Axis )
{
	switch (Axis)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		PlaceRotation.Roll = InFloat;
		break;
	case EAxis::Y:
		PlaceRotation.Pitch = InFloat;
		break;
	case EAxis::Z:
		PlaceRotation.Yaw = InFloat;
		break;
	default:
		break;
	}

	if (IsValid(RenderActor))
	{		
		RenderActor->SetActorRotation(PlaceRotation);
		UpdateActorPlacementArray();
		if (GEditor->GetSelectedActors()->IsSelected(RenderActor))
		{
			GEditor->SelectNone(true, true);
		}

	}
}


TOptional<float> ActorPlaceToolModel::GetRotationFloat ( EAxis::Type Axis ) const
{
	float RetVal{ 0 };
	switch (Axis)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		RetVal = PlaceRotation.Roll;
		break;
	case EAxis::Y:
		RetVal = PlaceRotation.Pitch;
		break;
	case EAxis::Z:
		RetVal = PlaceRotation.Yaw;
		break;
	default:
		break;
	}
	return RetVal;
}

void ActorPlaceToolModel::SetSeed ( const int32& Inint )
{
	Seed = Inint;
	UpdateActorPlacementArray();
}

void ActorPlaceToolModel::SetLineCount ( const int32& InLineCount )
{
	LineCount = InLineCount;
	UpdateActorPlacementArray();
}

void ActorPlaceToolModel::SetShouldRaycast ( const bool bInShouldRaycast )
{
	bShouldRaycast = bInShouldRaycast;
	UpdateActorPlacementArray();
}

void ActorPlaceToolModel::SelectRenderActor ()
{
	if(IsValid(RenderActor))
	{
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(RenderActor, true, true);
	}
}

void ActorPlaceToolModel::RaycastAllArray()
{
	if (bShouldRaycast)
	{
		if (UWorld* World = GEditor->GetEditorWorldContext().World())
		{
			const double DistanceToTrace { 10000.0 };
			ParallelFor(ActorPlacementArray.Num(), [&] (int32 Idx)
			{
				FHitResult HitResult;

				FVector StartPoint = ActorPlacementArray[Idx];
				StartPoint.Z = StartPoint.Z + DistanceToTrace;

				FVector EndPoint = ActorPlacementArray[Idx];
				EndPoint.Z = EndPoint.Z - DistanceToTrace;

				if (World->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_WorldStatic) )
				{
					ActorPlacementArray[Idx].Z = HitResult.Location.Z;
				}

			}, false);
		}
	}
	if (RenderActor)
	{
		RenderActor->UpdateActorPreview(ActorPlacementArray);
	}
}

void ActorPlaceToolModel::RenderActorMoved ( FVector InVec, FRotator InRot)
{
	PlaceVector = InVec;
	PlaceRotation = InRot;
	if (IsValid(RenderActor))
	{
		RenderActor->UpdateMainPoint(PlaceVector);
		UpdateActorPlacementArray();
	}
}

void ActorPlaceToolModel::UpdateRenderActorArray ( const TArray<FVector>& Vectors, const bool bShouldCheckDistance)
{
	if (RenderActor)
	{
		ActorPlacementArray.Empty();
		FVector RenderCenter { RenderActor->GetActorLocation() + RenderActor->GetActorRotation().RotateVector(RenderActor->BoxToDraw.FirstPoint) };
		FTransform RenderTrans = RenderActor->GetTransform();
		
		double Z = PlaceVector.Z;
		for (const FVector& Vec : Vectors)
		{
			bool bShouldAdd {true};
			FVector TempVec = RenderTrans.TransformVector(Vec);
			TempVec = TempVec + RenderCenter ;

			if (bShouldCheckDistance)
			{
				for (const FVector& ExistingVector : ActorPlacementArray)
				{
					if (FVector::DistSquared(ExistingVector, TempVec) < MinDistance * MinDistance)
					{
						bShouldAdd = false;
						break;
					}
				}
			}
			if (bShouldAdd)
			{
				
				ActorPlacementArray.Add(TempVec);
			}
		}

		RenderActor->UpdateActorPreview(ActorPlacementArray);
	}
}

void ActorPlaceToolModel::SpawnSingleActor()
{
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		if (ObjectToPlace)
		{
			FTransform Transform{ FTransform(PlaceRotation, PlaceVector, PlacementScale) };
			TSubclassOf<AActor> ObjToPlace;

			UE_LOG(LogTemp, Warning, TEXT("ObjPath :  %s"), *ObjectPath)
			UClass* Class = ObjectToPlace->GetClass();
			if (Cast<AActor>(ObjectToPlace))
			{
				UE_LOG(LogTemp, Warning, TEXT("ActorCanAct"));
			}

			if (UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>())
			{
				EditorActorSubsystem->SpawnActorFromObject(ObjectToPlace, PlaceVector, PlaceRotation);
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No Actor Selected")));
		}
	}

}

void ActorPlaceToolModel::SpawnActor(const FVector& InVector, const FRotator& InRotation)
{
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		if (ObjectToPlace)
		{
			if (UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>())
			{
				EditorActorSubsystem->SpawnActorFromObject(ObjectToPlace, InVector, InRotation);
			}
		}
	}
}

void ActorPlaceToolModel::BoxWidthChanged(float InBoxWidth)
{
	BoxWidth = InBoxWidth;
	BoxX = BoxWidth;
	BoxY = BoxWidth;
	UpdateNonUniformBox();

}

void ActorPlaceToolModel::BoxXChanged(double InBoxX)
{
	BoxX = InBoxX;
	UpdateNonUniformBox();
}

void ActorPlaceToolModel::BoxYChanged(double inBoxY)
{
	BoxY = inBoxY;
	UpdateNonUniformBox();
}

void ActorPlaceToolModel::BoxTypeChanged ()
{
	if (bUniformBoxSize)
	{
		BoxX = BoxWidth;
		BoxY = BoxWidth;
	}
	
	UpdateNonUniformBox();
	
}

void ActorPlaceToolModel::UpdateNonUniformBox()
{
	if (RenderActor)
	{
		RenderActor->SetBoxXY(BoxX, BoxY);
		UpdateActorPlacementArray();
	}
}

void ActorPlaceToolModel::UniformSpawnDistanceChanged(double InUniformSpawnDistance)
{
	UniformSpawnDistance = InUniformSpawnDistance;
	UpdateActorPlacementArray();

}

void ActorPlaceToolModel::UniformMethod()
{
	// test

	if (RenderActor)
	{
		//TArray<FVector> LocationsToSpawnActors;
		ActorPlacementArray.Empty();

		TArray<FVector> TempLocalVecs;
		//First point should be smaller than second
		FVector FirstPoint{ RenderActor->BoxToDraw.FirstPoint };
		FVector SecondPoint{ RenderActor->BoxToDraw.SecondPoint };

		double X{ DistanceFromEdge };
		double Y{ DistanceFromEdge };
		double Z{ 0.0 };
		// start center maybe?

		for (X; X <= BoxX - DistanceFromEdge; X += UniformSpawnDistance)
		{
			for (Y; Y <= BoxY - DistanceFromEdge; Y += UniformSpawnDistance)
			{
				TempLocalVecs.Add(FVector(X, Y, Z));
			}
			Y = DistanceFromEdge;
		}

		// adjust to center 
		if (TempLocalVecs.Num() > 0)
		{
			FVector First{ TempLocalVecs[0]};
			FVector Last{ TempLocalVecs[TempLocalVecs.Num() - 1] };

			double LilX = First.X + Last.X;
			double LilY = First.Y + Last.Y;

			double BX = (BoxX - LilX) / 2;
			double BY = (BoxY - LilY) / 2;

			for (FVector& Vec : TempLocalVecs)
			{
				Vec.X += BX;
				Vec.Y += BY;
			}
		}

		UpdateRenderActorArray(TempLocalVecs, false);
		//for (const FVector& Vec : TempLocalVecs)
		//{
		//	FVector TempVec{ FirstPoint + Vec };
		//	TempVec.Z = Z;
		//	ActorPlacementArray.Add(TempVec);
		//}

		//RenderActor->UpdateActorPreview(ActorPlacementArray);
	}



	//if (RenderActor && ObjectToPlace)
	//{
	//	//TArray<FVector> LocationsToSpawnActors;
	//	ActorPlacementArray.Empty();
	//
	//		//First point should be smaller than second
	//	FVector FirstPoint{ RenderActor->BoxToDraw.FirstPoint };
	//	FVector SecondPoint{ RenderActor->BoxToDraw.SecondPoint };
	//
	//	double X{ FirstPoint.X + UniformSpawnDistance };
	//	double Y{ FirstPoint.Y + UniformSpawnDistance };
	//	double Z{ PlaceVector.Z };
	//	// start center maybe?
	//
	//	for (X; X <= SecondPoint.X - UniformSpawnDistance; X += UniformSpawnDistance)
	//	{
	//		for (Y; Y <= SecondPoint.Y - UniformSpawnDistance; Y += UniformSpawnDistance)
	//		{
	//			ActorPlacementArray.Add(FVector(X, Y, Z));
	//		}
	//		Y = FirstPoint.Y + UniformSpawnDistance;
	//	}
	//	RenderActor->UpdateActorPreview(ActorPlacementArray);
	//}
}

void ActorPlaceToolModel::RandomDistanceChanged ( const double& InDouble, EMinMax MinMaxType )
{
	if (InDouble >= 0 )
	{
		switch (MinMaxType)
		{
		case Min:
			MinDistance = InDouble;
			if (MinDistance < 1.0)
			{
				MinDistance = 1.0;
			}
			if (MinDistance >= MaxDistance)
			{
				MaxDistance = MinDistance + 1.0;
			}
			break;
		case Max:
			MaxDistance = InDouble;
			if(MaxDistance <= 2.0)
			{
				MaxDistance = 2.0;
			}
			if(MaxDistance <= MinDistance)
			{
				MinDistance = MaxDistance -1.0;
			}
			break;
		default:
			break;
		}
	}
	UpdateActorPlacementArray();
}

double ActorPlaceToolModel::GetRandomDistance ( EMinMax MinMaxType )
{
	switch (MinMaxType)
	{
	case Min:
		return MinDistance;
	case Max:
		return MaxDistance;

	default:
		return 0.0;
	}

}

void ActorPlaceToolModel::RandomCountChanged ( const int32& InCount )
{
	RandomCount = InCount; 
	UpdateActorPlacementArray();
}

void ActorPlaceToolModel::UpdateActorPlacementArray ()
{
	switch (PlacementOption)
	{
	case UniformPlacement:
		UniformMethod();
		break;
	case RandomPathing:
		RandomMethodPathing();
		break;
	case RandomLineSim:
		RandomMethodLineSim();
		break;
	case RandomChaos:
		RandomMethodChaos();
		break;
	default:
		break;
	}
	RaycastAllArray();
}

void ActorPlaceToolModel::RandomMethodPathing()
{
	if (RenderActor)
	{
		/* 
		*	Box area = X * Y
		*	R = MaxDistance
		*	Pi = UE_PI 
		*	area = Pi * R *R
		*	Max Num = BoxArea / area
		*/


		int32 MinNum { FMath::FloorToInt32((BoxX * BoxY )/ (UE_PI * MaxDistance * MaxDistance * 0.5))};
		
		if (MinNum >= 5000)
		{
			if (FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(TEXT("WARNING: This is attempting to create over 5000 previews.\nAre you sure you want to continue?"))) == EAppReturnType::No)
			{
				return;
			}
		}
		
		//int32 MaxNum {  int32(FMath::Floor( MinDistance > 0 ? (BoxX * BoxY )/ MaxDistance : (BoxX * BoxY) ))};

		double X { 0.0 };
		double Y { 0.0 };
		double Z { 0.0 };

		double StartLocationX { RandomisationStatics::DSeededRandRange(Seed, 0.0, BoxX, 667458854, 102934745) };
		double StartLocationY { RandomisationStatics::DSeededRandRange(Seed, 0.0, BoxY, 253748596, 947565623) };

		TArray<FVector> LocationsToPlace;
		LocationsToPlace.Add(FVector(StartLocationX, StartLocationY, 0.0));

		bool bStillGoing {true};
		int32 Idx {0};
		int32 ModifierNum {0};
		int32 Count { 0 };
		while (bStillGoing)
		{
			if (LocationsToPlace.IsValidIndex(Idx))
			{
				double TempRand {RandomisationStatics::FSeededRandRange(Seed + Idx + ModifierNum, MinDistance, MaxDistance, 394875395, 768739047)};
				FVector TempVec {FVector::ZeroVector};
				 
				if (GenerateLocationRecursive(Seed + Idx + ModifierNum, LocationsToPlace[Idx], TempVec, TempRand, LocationsToPlace, 10, Count ))
				{
					LocationsToPlace.Add(TempVec);
					ModifierNum += 1;
				}
				else
				{
					++Idx;
					Count = 0;
					ModifierNum = 0;
				}
			}
			else
			{
				bStillGoing = false;
			}
		}
		UpdateRenderActorArray(LocationsToPlace, true);
	} 
}

bool ActorPlaceToolModel::GenerateLocationRecursive ( const int32& InSeed, const FVector& InVector, FVector& OutVector, double DistanceToPlace, const TArray<FVector>& VectorsToTest, const int32& MaxCount, int32& CurrentCount )
{
	if (CurrentCount <= MaxCount)
	{
		// Generate direction Fvec between -1 & 1
		
		double RandX = FMath::RandRange(-1.0, 1.0); 
		double RandY = FMath::RandRange(-1.0, 1.0);

		FVector DirectionVec {RandomisationStatics::VecSeededRandRange(InSeed + CurrentCount, -1.0, 1.0, true)};
		DirectionVec.Normalize();

		// check if distance and direction vec can be placed
		FVector TempVector { InVector + (DirectionVec * DistanceToPlace) };
		bool bCanBePlaced {true};
		if (TempVector.X < BoxX && TempVector.X > 0.0 && TempVector.Y < BoxY && TempVector.Y > 0.0)
		{
			//not out of bounds
			for (const FVector& VectorToTest : VectorsToTest)
			{
				if ((FVector::DistSquared2D(TempVector, VectorToTest) <= MinDistance * MinDistance))
				{
					bCanBePlaced = false;
					break;
				}
			}
		}
		else
		{
			// Out of bounds
			bCanBePlaced = false;
		}

		// if no, +1 count and call this function again
		if (!bCanBePlaced)
		{
			++CurrentCount;
			
			bCanBePlaced = GenerateLocationRecursive(InSeed, InVector, TempVector, DistanceToPlace, VectorsToTest, MaxCount, CurrentCount);
		}
		// if yes, return true and set OutVector to new vec
		if(bCanBePlaced)
		{
			OutVector = TempVector;
			return true;
		}
	}
	return false;
}

void ActorPlaceToolModel::RandomMethodLineSim()
{

	TArray<FVector> Vectors;
	double Z = 0.0;
	if (RandomCount >= 1)
	{
		Vectors = RandomisationStatics::VecSeededandRangeCountLineSim(Seed, RandomCount, true, LineCount, 0, 1000, true , BoxX, BoxY, 2000);
	}

	UpdateRenderActorArray(Vectors, true);

}

void ActorPlaceToolModel::RandomMethodChaos()
{
	TArray<float> XFlts = RandomisationStatics::FltChaosMachine(Seed, RandomCount);
	TArray<float> YFlts = RandomisationStatics::FltChaosMachine(Seed+2, RandomCount);

	TArray<FVector> Vectors;
	Vectors.Init(FVector::ZeroVector, RandomCount);
	ParallelFor(RandomCount, [&] (int32 Idx)
	{
		double X = BoxX  * XFlts[Idx];
		double Y = BoxY  * YFlts[Idx];

		double Z = 0; // keep Z at zero for now

		Vectors[Idx] = FVector(X, Y, Z);

	}, false);

	UpdateRenderActorArray(Vectors, true);
	
}

void ActorPlaceToolModel::RandomiseSeed ()
{
	Seed = RandomisationStatics::I32Rand();
	UpdateActorPlacementArray();
}

void ActorPlaceToolModel::SpawnActors()
{
	UpdateActorPlacementArray();

	if (ObjectToPlace)
	{
		for (const FVector& LocationToSpawn : ActorPlacementArray)
		{
			SpawnActor(LocationToSpawn, PlaceRotation);
		}
	}
}

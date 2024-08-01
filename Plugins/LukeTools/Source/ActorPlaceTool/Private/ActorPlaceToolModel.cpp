// Luke Jackson 29/07/2024

#include "ActorPlaceToolModel.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Subsystems/EditorActorSubsystem.h"

ActorPlaceToolModel::ActorPlaceToolModel()
{
	//CreateDefaultSubobject()
	//RenderComp = NewObject<URenderComp>();
	
}

void ActorPlaceToolModel::Init(SActorPlaceToolView* InView)
{
	ToolView = InView;
	RenderActor = NewObject<ARenderActor>();
	if (ToolView)
	{
		FAssetRegistryModule& RAssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		AssetRegistryModule = &RAssetRegistryModule;
		ToolView->InitView();
	}

}

void ActorPlaceToolModel::DeInit()
{
	ToolView = nullptr;
	RenderActor->Destroy();
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

void ActorPlaceToolModel::TestButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Button Pressed"));
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
}

void ActorPlaceToolModel::TestVisualiser()
{
	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f. Z: %f"), PlaceVector.X, PlaceVector.Y, PlaceVector.Z);
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *World->GetFName().ToString());
		DrawDebugPoint(World, PlaceVector, 5.f, FColor::Green, false, 2.f);

	}
	if (IsValid(RenderActor))
	{
		RenderActor->RenderComp->TestFunction();
	}

}

void ActorPlaceToolModel::TestSpawnActor()
{
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		if (ObjectToPlace)
		{
			FTransform Transform{ FTransform(PlaceRotation, PlaceVector, PlacementScale) };
			TSubclassOf<AActor> ObjToPlace;
			//ConstructorHelpers::FObjectFinder<UBlueprint> BPFind(*ObjectPath);
			//BPFind.Object->GeneratedClass();
			//ObjectToPlace->Class
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
			//ObjToPlace = Cast<AActor>(ObjectToPlace)->GetClass();
			//UE_LOG(LogTemp, Warning, TEXT(" %s"), *ObjToPlace->GetPathName());
			//if (AActor* NewObj = World->SpawnActor<AActor>(ObjToPlace, Transform))
			//UE_LOG(LogTemp, Warning, TEXT("Obj :  %s"), *NewObj->GetFName().ToString())
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No Actor Selected")));
		}
	}

}


ARenderActor::ARenderActor()
{
	RenderComp = CreateDefaultSubobject<URenderComp>(FName(TEXT("RenderComponent")));
}
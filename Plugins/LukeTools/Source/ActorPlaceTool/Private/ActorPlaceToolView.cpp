#include "ActorPlaceToolView.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "ActorPlaceToolModel.h"
//#include "SAssetSearchBox.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "PropertyCustomizationHelpers.h"

void SActorPlaceToolView::Construct(const FArguments& InArgs)
{
	ToolModel = InArgs._InToolModel;
	if (ToolModel)
	{
		ToolModel->Init(this);

	}
	
}

void SActorPlaceToolView::InitView()
{
	if (!ToolModel->bAssetRegistryLoaded)
	{

		ChildSlot
		[
			//Main vertical box
			SNew(SVerticalBox)

			//First vertical box for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Asset registry Loading")))
		//	.Font(TitleTextFont)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FColor::White)
		]
		];
		ToolModel->IsFinishedLoading();
	}
	else
	{
		CreateToolBox();
	}

}

void SActorPlaceToolView::CreateToolBox()
{
	ChildSlot
		[
			//Main vertical box
			SNew(SVerticalBox)

			//First vertical box for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Assets Loaded - Put more tool here")))
		//	.Font(TitleTextFont)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("ButtonToPress")))
		.OnClicked_Lambda([this]()
			{
				ToolModel->TestButtonPressed();
				return FReply::Handled();
			})
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Assets Loaded - Put more tool here")))
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SObjectPropertyEntryBox)
			.OnObjectChanged_Lambda([this](const FAssetData& InAssetData) 
			{
					ToolModel->ObjectChanged(InAssetData);
				UE_LOG(LogTemp, Warning, TEXT("OBJECT CHANGERD"));
			})
			.ObjectPath_Lambda([this]()
			{
				return ToolModel->GetObjectPath();
			})

		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SVectorInputBox)
				.AllowSpin(true)
				.X_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::X); })
				.Y_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::Y); })
				.Z_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::Z); })
				.OnXChanged_Lambda([this](TOptional<float> InFloat)
					{
						ToolModel->SetPlaceVectorFloat(InFloat, EAxis::X);
						//FVector NewVec{ FVector(ToolModel->GetPlaceVectorFloat(EAxis::X).Get(0.f), ToolModel->GetPlaceVectorFloat(EAxis::Y).Get(0.f), ToolModel->GetPlaceVectorFloat(EAxis::Z).Get(0.f)) };
						//NewVec.X = InFloat.Get(0.f);
						//ToolModel->SetPlaceVector(NewVec);
					})
				.OnYChanged_Lambda([this](TOptional<float> InFloat)
				{
					ToolModel->SetPlaceVectorFloat(InFloat, EAxis::Y);
				})
				.OnZChanged_Lambda([this](TOptional<float> InFloat)
				{
					ToolModel->SetPlaceVectorFloat(InFloat, EAxis::Z);
				})

				//.OnVectorChanged_Lambda([this](const UE::Math::TVector<float>& InVector)
				//{
				//	FVector NewVec(InVector.X, InVector.Y, InVector.Z);
				//	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f. Z: %f"), NewVec.X, NewVec.Y, NewVec.Z);
				//	ToolModel->SetPlaceVector(NewVec);
				//})
			]

			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("VisualiserButton")))
				.OnClicked_Lambda([this]()
					{
						
						ToolModel->TestVisualiser();
						return FReply::Handled();
					})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("VisualiserButton")))
				.OnClicked_Lambda([this]()
					{
						
						ToolModel->TestSpawnActor();
						return FReply::Handled();
					})
			]
				
		]
	];
	
}


//void SActorPlaceToolView::Construct(const FArguments& InArgs, ActorPlaceToolModel* ToolModel)
//{
//	ChildSlot
//		[
//			//Main vertical box
//			SNew(SVerticalBox)
//
//			//First vertical box for title text
//		+ SVerticalBox::Slot()
//		.AutoHeight()
//		[
//			SNew(STextBlock)
//			.Text(FText::FromString(TEXT("Tool Model Aquired")))
//		//	.Font(TitleTextFont)
//		.Justification(ETextJustify::Center)
//		.ColorAndOpacity(FColor::White)
//		]
//		];
//
//}
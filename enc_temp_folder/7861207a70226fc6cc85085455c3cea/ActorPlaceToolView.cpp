#include "ActorPlaceToolView.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "ActorPlaceToolModel.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SComboBox.h"
#include "SEnumCombo.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SRotatorInputBox.h"

SActorPlaceToolView::SActorPlaceToolView()
{
	RandomEnum = StaticEnum<EPlacementMethod>();
		
	SplitterBackgroundColorBrush.TintColor = FSlateColor(FColor(2, 2, 2));
	SplitterBackgroundColorBrush.DrawAs = ESlateBrushDrawType::Box;

	BoxBackgroundColorBrush.TintColor = FSlateColor(FColor(1, 1, 1));
	BoxBackgroundColorBrush.DrawAs    = ESlateBrushDrawType::Box;
}

SActorPlaceToolView::~SActorPlaceToolView()
{
	if (ToolModel)
	{
		ToolModel->DeInit();
	
	}

}


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
	if (!ToolModel->bAssetRegistryLoaded) // waits for asset registry to load before view is enabled
	{

		ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Asset registry Loading")))
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
		SNew(SScrollBox)
		+SScrollBox::Slot()
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Actor Placement Tool")))
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::White)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SObjectPropertyEntryBox)
				.OnObjectChanged_Lambda([this](const FAssetData& InAssetData) 
				{
						ToolModel->ObjectChanged(InAssetData);
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
				.Padding(10.f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Location: ")))
				]

				+SHorizontalBox::Slot()
					.MaxWidth(300.f)
				[
					SNew(SVectorInputBox)
					.AllowSpin(true)
					.X_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::X); })
					.Y_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::Y); })
					.Z_Lambda([this]() { return ToolModel->GetPlaceVectorFloat(EAxis::Z); })
					.OnXChanged_Lambda([this](TOptional<float> InFloat)
						{
							ToolModel->SetPlaceVectorFloat(InFloat, EAxis::X);
						})
					.OnYChanged_Lambda([this](TOptional<float> InFloat)
					{
						ToolModel->SetPlaceVectorFloat(InFloat, EAxis::Y);
					})
					.OnZChanged_Lambda([this](TOptional<float> InFloat)
					{
						ToolModel->SetPlaceVectorFloat(InFloat, EAxis::Z);
					})


				]
			]
			+SVerticalBox::Slot()
				.AutoHeight()
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Rotation:")))
				]

				+SHorizontalBox::Slot()
				.MaxWidth(300.f)
				[
					SNew(SRotatorInputBox)
					.AllowSpin(true)
					.Pitch_Lambda([this]() { return ToolModel->GetRotationFloat(EAxis::X); } )
					.Roll_Lambda([this]()  { return ToolModel->GetRotationFloat(EAxis::Y); } )
					.Yaw_Lambda([this]()   { return ToolModel->GetRotationFloat(EAxis::Z); } )
					.OnPitchChanged_Lambda([this](float InFLoat)
					{
						ToolModel->SetRotationFloat(InFLoat, EAxis::X);
					} )
					.OnRollChanged_Lambda([this](float InFLoat)
					{
						ToolModel->SetRotationFloat(InFLoat, EAxis::Y);
					} )
					.OnYawChanged_Lambda([this](float InFLoat)
					{
						ToolModel->SetRotationFloat(InFLoat, EAxis::Z);
					} )
				]
			]

			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Select Movement Actor")))
					.OnClicked_Lambda([this]()
					{
						ToolModel->SelectRenderActor();
						return FReply::Handled();
					})
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Spawn Single Actor")))
					.OnClicked_Lambda([this]()
						{
						
							ToolModel->SpawnSingleActor();
							return FReply::Handled();
						})
				]
				
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SExpandableArea)
				.AreaTitle(FText::FromString(TEXT("Box Settings")))
				.BodyBorderImage(&BoxBackgroundColorBrush)
					.BodyContent()
				[
			
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(10.f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Uniform Box")))
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SCheckBox)
							.IsChecked_Lambda([this]() 
								{

									return ToolModel->bUniformBoxSize ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
								})
							.OnCheckStateChanged_Lambda([this](ECheckBoxState State) 
								{
									State == ECheckBoxState::Checked ? ToolModel->bUniformBoxSize = true : ToolModel->bUniformBoxSize = false;
									ToolModel->BoxTypeChanged();
									InitView();
								})

						]
					]
					+SVerticalBox::Slot()
						[
							SNew(SBorder)
							.BorderImage(&SplitterBackgroundColorBrush)
							.Padding(0.0f)
							[
								SNew(STextBlock).Text(FText::FromString(TEXT("")))
							]
						]
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						CreateUniformPlacementBox()
					
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SExpandableArea)
				.AreaTitle(FText::FromString(TEXT("Multi Place")))
				.BodyBorderImage(&BoxBackgroundColorBrush)
				.BodyContent()
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(10.f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Should Raycast:")))
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SCheckBox)
							.IsChecked_Lambda([this]() 
								{
									return ToolModel->GetShouldRaycast() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
								})
							.OnCheckStateChanged_Lambda([this](ECheckBoxState State) 
								{
									State == ECheckBoxState::Checked ? ToolModel->SetShouldRaycast(true) : ToolModel->SetShouldRaycast(false);
								})

						]
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)

						+SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.FillWidth(0.1f)
						.Padding(10.f)
						.AutoWidth()
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Distribution Method")))
						]

						+SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.AutoWidth()
						[
							CreateTypeSelectionEnumBox()	
						]
					]
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBorder)
						.BorderImage(&SplitterBackgroundColorBrush)
						.Padding(0.0f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("")))
						]
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					[
						CreateUniformRandomPlacement()
					]
				]
			]
		]
	];
	
}

TSharedRef<SWidget> SActorPlaceToolView::CreateUniformPlacementBox()
{
	TSharedRef<SWidget> ReturnWidget = AsShared();
	if (ToolModel->bUniformBoxSize)
	{

		ReturnWidget =  SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Box Width")))
		]

		+ SHorizontalBox::Slot()
			.AutoWidth()
		[
			SNew(SSpinBox<double>)
			.OnValueChanged_Lambda([this](double NewFloat)
			{
				ToolModel->BoxWidthChanged(NewFloat);
				})
			.Value_Lambda([this]()
			{
				return ToolModel->GetBoxWidth();
			})
		];
		
	}
	else
	{
		ReturnWidget =  
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(10.f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Box X ( CM )")))
				]

				+ SHorizontalBox::Slot()
					.AutoWidth()
				[
					SNew(SSpinBox<double>) // Not updating dynamically for some reason
					.OnValueChanged_Lambda([this](double NewDouble)
					{
						ToolModel->BoxXChanged(NewDouble);
						})
					.Value_Lambda([this]()
					{
						return ToolModel->GetBoxX();
					})
				]
			]
			
			+SVerticalBox::Slot()
			
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(10.f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Box Y ( CM )")))
				]

				+ SHorizontalBox::Slot()
					.AutoWidth()
				[
					SNew(SSpinBox<double>)
					.OnValueChanged_Lambda([this](double NewDouble)
					{
						ToolModel->BoxYChanged(NewDouble);
						})
					.Value_Lambda([this]()
					{
						return ToolModel->GetBoxY();
					})
				]
			];

	}
	return ReturnWidget;
}

TSharedRef<SWidget> SActorPlaceToolView::CreateUniformRandomPlacement ()
{
	switch (ToolModel->PlacementOption)
	{

	case UniformPlacement:
		return 
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Uniform Distance:"))	)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SSpinBox<double>)
					.OnValueChanged_Lambda([this](double NewFloat)
					{
						ToolModel->UniformSpawnDistanceChanged(NewFloat);
					})
					.Value_Lambda([this]()
					{
						return ToolModel->GetUniformSpawnDistance();
					})
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					CreateSpawnActorButton()
				]
			];
			
	case RandomPathing:
		return 

				SNew(SVerticalBox)
					
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
						
					.MaxWidth(80)
					.Padding(10.f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Min Distance:")))
					]
					+SHorizontalBox::Slot()
						.AutoWidth()
					[
						SNew(SSpinBox<double>)
						.OnValueChanged_Lambda([this](double NewDouble)
						{
							ToolModel->RandomDistanceChanged(NewDouble, EMinMax::Min);
						})
						.Value_Lambda([this]()
						{
							return ToolModel->GetRandomDistance(EMinMax::Min);
						})	
					]
				]

				+SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MaxWidth(80)
					.Padding(10.f)
					[	
						SNew(STextBlock).Text(FText::FromString(TEXT("Max Distance:")))
					]
					+SHorizontalBox::Slot()
						.AutoWidth()
					[
						SNew(SSpinBox<double>)
						.OnValueChanged_Lambda([this](double NewDouble)
						{
							ToolModel->RandomDistanceChanged(NewDouble, EMinMax::Max);
						})
						.Value_Lambda([this]()
						{
							return ToolModel->GetRandomDistance(EMinMax::Max);
						})
					]
				]
				+SVerticalBox::Slot()
				[
					CreateSeedRow()
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.Padding(10.f)
				[
					CreateSpawnActorButton()
				];
		break;
	case RandomLineSim:
		return SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.Content()
			[
				SNew(SVerticalBox)
					
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MaxWidth(80)
					.Padding(10.f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Min Distance:")))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSpinBox<double>)
						.OnValueChanged_Lambda([this](double NewDouble)
						{
							ToolModel->RandomDistanceChanged(NewDouble, EMinMax::Min);
						})
						.Value_Lambda([this]()
						{
							return ToolModel->GetRandomDistance(EMinMax::Min);
						})
					]
				]

				+SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MaxWidth(80)
					.Padding(10.f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Count")))
					]

					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSpinBox<int32>)
						.OnValueChanged_Lambda([this](int32 NewCount)
						{
							ToolModel->RandomCountChanged(NewCount);
						})
						.Value_Lambda([this]()
						{
							return ToolModel->GetRandomCount();
						})
					]
				]
				+SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.MaxWidth(80)
					.Padding(10.f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Line Count:")))
					]

					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSpinBox<int32>)
						.MinValue(1)
						.OnValueChanged_Lambda([this](int32 NewInt)
						{
							ToolModel->SetLineCount(NewInt);

						})
						.Value_Lambda([this]()
						{
							return ToolModel->GetLineCount();	
						})
					]
				]
				+SVerticalBox::Slot()
				[
					CreateSeedRow()
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.Padding(10.f)
				[
					CreateSpawnActorButton()
				]
			]
		];

		break;
	case RandomChaos:
		return 
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.Content()
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.MaxWidth(80)
						.Padding(10.f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Min Distance:")))
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSpinBox<double>)
							.OnValueChanged_Lambda([this](double NewDouble)
							{
								ToolModel->RandomDistanceChanged(NewDouble, EMinMax::Min);
							})
							.Value_Lambda([this]()
							{
								return ToolModel->GetRandomDistance(EMinMax::Min);
							})
						]
					]
						
					+ SVerticalBox::Slot()
					
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.MaxWidth(80)
						.Padding(10.f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Count:")))
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSpinBox<int32>)
							.OnValueChanged_Lambda([this](int32 NewCount)
							{
								ToolModel->RandomCountChanged(NewCount);
							})
							.Value_Lambda([this]()
							{
								return ToolModel->GetRandomCount();
							})
						]
						
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						CreateSeedRow()
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					.HAlign(EHorizontalAlignment::HAlign_Left)
					.Padding(10.f)
					[
						CreateSpawnActorButton()
					]
				]
			];
		break;
	default:
		break;
		
	}
	
	return SNew(STextBlock).Text(FText::FromString(TEXT("An Error has Occured.")));
}

TSharedRef<SWidget> SActorPlaceToolView::CreateTypeSelectionEnumBox ()
{
	return SNew(SEnumComboBox, RandomEnum)
			.ContentPadding(4.f)
			.CurrentValue_Lambda([this]()
			{
				return ToolModel->PlacementOption;
			})
			.OnEnumSelectionChanged_Lambda([this](int32 Value, ESelectInfo::Type SelectionType)
			{
				ToolModel->PlacementOption = EPlacementMethod(Value);
				ToolModel->UpdateActorPlacementArray();
				InitView();
			});
}

TSharedRef<SWidget> SActorPlaceToolView::CreateSpawnActorButton ()
{
	return SNew(SButton)
	.Text(FText::FromString(TEXT("Spawn Actor Randomly")))
	.OnClicked_Lambda([this]()
	{
		ToolModel->SpawnActors();
		return FReply::Handled();
	});
}

TSharedRef<SWidget> SActorPlaceToolView::CreateSeedRow ()
{
	return
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.MaxWidth(80)
		.Padding(10.f)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Seed:")))
		]

		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpinBox<int32>)
			.OnValueChanged_Lambda([this](int32 NewInt)
			{
				ToolModel->SetSeed(NewInt);

			})
			.Value_Lambda([this]()
			{
				return ToolModel->GetSeed();	
			})
		]
		+SHorizontalBox::Slot()
			.Padding(4.f)
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Randomise Seed")))
			.OnClicked_Lambda([this]()
			{
				ToolModel->RandomiseSeed();
				return FReply::Handled();
			})
		];
}
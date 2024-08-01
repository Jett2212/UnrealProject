#pragma once

#include "Widgets/SCompoundWidget.h"


class ActorPlaceToolModel;

class SActorPlaceToolView : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SActorPlaceToolView) {}

	//SLATE_ARGUMENT(TArray<TSharedPtr <FAssetData> >, AssetsDataToStore)
	SLATE_ARGUMENT(ActorPlaceToolModel*, InToolModel)

	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
	void InitView();
	void CreateToolBox();
	//void Construct(const FArguments& InArgs, ActorPlaceToolModel* InToolModel);
	//inline void SetToolModel(ActorPlaceToolModel* InToolModel) { ToolModel = InToolModel; }
	//TestButtonPressed();


	ActorPlaceToolModel* ToolModel { nullptr };
};
#pragma once

#include "Widgets/SCompoundWidget.h"


class ActorPlaceToolModel;

class SActorPlaceToolView : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SActorPlaceToolView) {}

	SLATE_ARGUMENT(ActorPlaceToolModel*, InToolModel)

	SLATE_END_ARGS()
public:

	SActorPlaceToolView();
	~SActorPlaceToolView();

	void Construct(const FArguments& InArgs);
	void InitView();

	ActorPlaceToolModel* ToolModel { nullptr };

private:
	void CreateToolBox();
	
	TSharedRef<SWidget> CreateUniformPlacementBox();
	TSharedRef<SWidget> CreateUniformRandomPlacement();

	TSharedRef<SWidget> CreateTypeSelectionEnumBox();

	TSharedRef<SWidget> CreateSpawnActorButton();
	TSharedRef<SWidget> CreateSeedRow();

	UEnum* RandomEnum {nullptr};

	FSlateBrush BoxBackgroundColorBrush;
	FSlateBrush SplitterBackgroundColorBrush;
};
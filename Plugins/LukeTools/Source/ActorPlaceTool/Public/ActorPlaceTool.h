// Luke Jackson 28/07/2024

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SlateFwd.h"
#include "HAL/IConsoleManager.h"
#include "Input/Reply.h"
#include "Framework/Commands/Commands.h"
#include "ActorPlaceToolModel.h"

class FUICommandList;
class FMenuBuilder;
class FToolBarBuilder;

class FActorPlaceToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private: 
	void ActorPlaceToolMenu();
	TSharedRef<SDockTab> OnSpawnActorPlaceToolTab(const FSpawnTabArgs& SpawnTabArgs);
	void RegisterActorPlaceToolTab();
	ActorPlaceToolModel* ToolModel {nullptr};
	TSharedPtr<SActorPlaceToolView> ActorViewTab {nullptr};
	
};
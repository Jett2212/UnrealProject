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
	
	// Add buttons to menus

	//void AddMenuEntryy(FName NewEntryName, const TAttribute<FText>& InEntryLabel, const TAttribute<FText>& InEntryToolTip, const TAttribute<FSlateIcon>& InSlateIcon, FExecuteAction EntryAction, const FName SectionName = NAME_None, const TAttribute<FText>& InSectionLabel = INVTEXT(""), const FToolMenuInsert InSectionPosition = FToolMenuInsert());
private: 
	void ActorPlaceToolMenu();
	TSharedRef<SDockTab> OnSpawnActorPlaceToolTab(const FSpawnTabArgs& SpawnTabArgs);
	void RegisterActorPlaceToolTab();
//	TSharedPtr<SActorPlaceToolView> ConfigView();
	ActorPlaceToolModel* ToolModel {nullptr};
	TSharedPtr<SActorPlaceToolView> ActorViewTab {nullptr};
	//void CreateMenu();
	//void AddMenuEx();w
	//
	//const FName LevelMenuName{ TEXT("LevelEditor.MainMenu") };
	//const FName MenuName{ TEXT("LukeTools") };
};




//class FTestTest : public TCommands<FTestTest>
//{
//public:
//	/**  constructor */
//	FTestTest();
//
//	/** List of all of the main frame commands */
//	static TSharedRef< FUICommandList > ActionList;
//
//	TSharedPtr< FUICommandInfo > TestCommand;
//
//	virtual void RegisterCommands() override;
//
//
//private:
//	void TestCommandPre();
//	bool DefaultCanExecuteAction() { return true; }
//	/** Console command for toggling full screen.  We need this to expose the full screen toggle action to
//		the game UI system for play-in-editor view ports */
//	//FAutoConsoleCommand ToggleFullscreenConsoleCommand;
//
//	//void RegisterLayoutCommands();
//};
//

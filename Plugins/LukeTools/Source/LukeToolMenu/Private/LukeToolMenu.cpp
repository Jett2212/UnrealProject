// Copyright Epic Games, Inc. All Rights Reserved.

#include "LukeToolMenu.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenuEntry.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Commands/UIAction.h"

/////////////////////////////////////////////////

//#include "Misc/MessageDialog.h"
//#include "HAL/FileManager.h"
//#include "Misc/CommandLine.h"
//#include "Misc/Paths.h"
//#include "Misc/App.h"
//#include "Modules/ModuleManager.h"
//#include "Widgets/DeclarativeSyntaxSupport.h"
//#include "Widgets/SWindow.h"
//#include "Framework/Application/SlateApplication.h"
//#include "Framework/Commands/UICommandList.h"
//#include "Framework/Docking/TabManager.h"
//#include "Interfaces/IMainFrameModule.h"
//#include "AboutScreen.h"
//#include "CreditsScreen.h"
//#include "DesktopPlatformModule.h"
//#include "ISourceControlModule.h"
////#include "ISourceControlWindowsModule.h"
////#include "GameProjectGenerationModule.h"
//#include "Toolkits/GlobalEditorCommonCommands.h"
//#include "Logging/TokenizedMessage.h"
//#include "Logging/MessageLog.h"
//#include "SourceCodeNavigation.h"
//#include "ISettingsModule.h"
//#include "Interfaces/IProjectManager.h"
//#include "Interfaces/ITargetPlatform.h"
//#include "Interfaces/ITargetPlatformManagerModule.h"
//#include "PlatformInfo.h"
//#include "Styling/AppStyle.h"
//#include "Settings/EditorExperimentalSettings.h"
//#include "CookerSettings.h"
//#include "UnrealEdMisc.h"
//#include "Editor/UnrealEdEngine.h"
//#include "Editor/Transactor.h"
//#include "Preferences/UnrealEdOptions.h"
//#include "UnrealEdGlobals.h"
//#include "FileHelpers.h"
//#include "EditorAnalytics.h"
//#include "LevelEditor.h"
//
//#include "Misc/ConfigCacheIni.h"
//#include "Widgets/Docking/SDockTab.h"
//#include "Framework/Commands/GenericCommands.h"
//#include "Dialogs/SOutputLogDialog.h"
//#include "Dialogs/Dialogs.h"
////#include "IUATHelperModule.h"
////#include "Menus/LayoutsMenu.h"
//#include "TargetReceipt.h"
//#include "IDocumentation.h"
//
//#include "Settings/EditorSettings.h"
//#include "AnalyticsEventAttribute.h"
//#include "Kismet2/DebuggerCommands.h"
//#include "GameMapsSettings.h"



//////////////////////////////




#define LOCTEXT_NAMESPACE "FActorPlaceToolModule"

void FLukeToolMenuModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	CreateMenu();
}

void FLukeToolMenuModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FLukeToolMenuModule::CreateMenu()
{

	TSharedPtr<FUICommandList> PluginCommands = MakeShareable(new FUICommandList);


	FLevelEditorModule& LevelEditorModule{ FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor") };

	//BaseCommands
	//General
	//Asset


	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(LevelMenuName);

	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);


	FToolMenuEntry& BuildEntry =
		Section.AddSubMenu(
			MenuName,
			LOCTEXT("LukeTools", "LukeTools"),
			LOCTEXT("LukeTools_ToolTip", "A Menu for tools"),
			FNewToolMenuChoice()
		);

	BuildEntry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);

	//UToolMenu* TestTestMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.TestTest");
	//FToolMenuSection& TTSection{ TestTestMenu->AddSection(FName(TEXT("SectionSection")), INVTEXT("Custom Section"), FToolMenuInsert()) };

	//TTSection.AddEntry(FToolMenuEntry::InitMenuEntry(
	//	FName(TEXT("CustomButton")),
	//	INVTEXT("CustomButton"),
	//	INVTEXT("Tooltip"),
	//	FSlateIcon(),
	//	FExecuteAction::CreateRaw(this, &FActorPlaceToolModule::AddMenuExtension)
	//));

	AddMenuEntry(FName("TestButton"), INVTEXT("TestButton"), INVTEXT("TestButton"), FSlateIcon(), FExecuteAction::CreateRaw(this, &FLukeToolMenuModule::AddMenuEx));
}

void FLukeToolMenuModule::AddMenuEx()
{
	UE_LOG(LogTemp, Warning, TEXT("New Press Detected"));

}

void FLukeToolMenuModule::AddMenuEntry(FName NewEntryName, const TAttribute<FText>& InEntryLabel, const TAttribute<FText>& InEntryToolTip, const TAttribute<FSlateIcon>& InSlateIcon, FExecuteAction EntryAction, const FName SectionName/* = NAME_None*/, const TAttribute<FText>& InSectionLabel/* = INVTEXT("")*/, const FToolMenuInsert InSectionPosition/* = FToolMenuInsert()*/)
{

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(FName(LevelMenuName.ToString() + TEXT(".") + MenuName.ToString()));

	FToolMenuSection& Section = Menu->FindOrAddSection(SectionName, InSectionLabel, InSectionPosition);

	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		NewEntryName,
		InEntryLabel,
		InEntryToolTip,
		InSlateIcon,
		EntryAction

	));

}

void FLukeToolMenuModule::PrintTest()
{
	UE_LOG(LogTemp, Warning, TEXT("PrintTest"));
}
//TSharedRef< FUICommandList > FTestTest::ActionList(new FUICommandList());
//
//void FTestTest::RegisterCommands()
//{
//	// Some commands cannot be processed in a commandlet or if the editor is started without a project
//	//if (!IsRunningCommandlet() && FApp::HasProjectName() && !IsRunningDedicatedServer())
//	{
//		// The global action list was created at static initialization time. Create a handler for otherwise unhandled keyboard input to route key commands through this list.
//		//FSlateApplication::Get().SetUnhandledKeyDownEventHandler(FOnKeyEvent::CreateStatic(&FMainFrameActionCallbacks::OnUnhandledKeyDownEvent));
//	}
//
//	// Make a default can execute action that disables input when in debug mode
//	FCanExecuteAction DefaultExecuteAction = FCanExecuteAction::CreateStatic(&FTestTest::DefaultCanExecuteAction);
//
//	UI_COMMAND(TestCommand, "Test", "Saves all unsaved levels and assets to disk", EUserInterfaceActionType::Button, FInputChord());
//	ActionList->MapAction(TestCommand, FExecuteAction::CreateStatic(&FTestTest::TestCommandPre), DefaultExecuteAction);
//
//}
//
//void FTestTest::TestCommandPre()
//{
//
//
//}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLukeToolMenuModule, LukeToolMenu)
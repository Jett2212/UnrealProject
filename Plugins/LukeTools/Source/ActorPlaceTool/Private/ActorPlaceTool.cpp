// Luke Jackson

#include "ActorPlaceTool.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenuEntry.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Commands/UIAction.h"
#include "LukeToolMenu.h"
#include "ActorPlaceToolView.h"
#include "Framework/Docking/TabManager.h"

//////////////////////////////




#define LOCTEXT_NAMESPACE "FActorPlaceToolModule"

void FActorPlaceToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterActorPlaceToolTab();

	ToolModel = new ActorPlaceToolModel() ;

	if (FModuleManager::Get().IsModuleLoaded("LukeToolMenu"))
	{
		FLukeToolMenuModule& LukeToolMenuModule{ FModuleManager::GetModuleChecked< FLukeToolMenuModule>("LukeToolMenu")};
		LukeToolMenuModule.AddMenuEntry(FName("ActorPlaceTool"), INVTEXT("ActorPlaceTool"), INVTEXT("ActorPlaceTool"), FSlateIcon(), FExecuteAction::CreateRaw(this, &FActorPlaceToolModule::ActorPlaceToolMenu));
	}
}

void FActorPlaceToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FActorPlaceToolModule::ActorPlaceToolMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Actor Window popup here"));

	FGlobalTabmanager::Get()->TryInvokeTab(FName("ActorPlaceToolTab"));

}

void FActorPlaceToolModule::RegisterActorPlaceToolTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("ActorPlaceToolTab"),
		FOnSpawnTab::CreateRaw(this, &FActorPlaceToolModule::OnSpawnActorPlaceToolTab))
		.SetDisplayName(FText::FromString(TEXT("Actor Place Tool")));

}

TSharedRef<SDockTab> FActorPlaceToolModule::OnSpawnActorPlaceToolTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedRef<SDockTab> RetVal =
		SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SActorPlaceToolView)
			.InToolModel(ToolModel)
		];
	
	return RetVal;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FActorPlaceToolModule, ActorPlaceTool)
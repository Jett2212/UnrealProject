// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActorPlaceTool : ModuleRules
{
	public ActorPlaceTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "EditorConfig",
                "EditorFramework",
                "UnrealEd",
                "ActorPickerMode",
                "SceneDepthPickerMode",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ToolMenus",
                "EditorFramework",
				"LukeToolMenu",
				"EditorWidgets",
                "Randomisation",
				// ... add private dependencies that you statically link with here ...	


				"ApplicationCore",
                "Engine",
                "EngineSettings",
                "InputCore",
                "RHI",
                "RenderCore",
                "Slate",
                "SlateCore",
                "SourceControl",
                "SourceControlWindows",
                "TargetPlatform",
                "DesktopPlatform",
                "EditorFramework",
                "UnrealEd",
                "WorkspaceMenuStructure",
                "MessageLog",
                "UATHelper",
                "TranslationEditor",
                "Projects",
                "DeviceProfileEditor",
                "UndoHistoryEditor",
                "Analytics",
                "ToolMenus",
                "LauncherServices",
                "InterchangeCore",
                "InterchangeEngine",
                "ToolWidgets",

                                "AppFramework",
                "Core",
                "CoreUObject",
                "ApplicationCore",
                "EditorStyle",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",
                "EditorWidgets",
                "Documentation",
                "ConfigEditor",
                "SceneOutliner",
                "DesktopPlatform",
                "PropertyPath",
                "ToolWidgets",
                "WidgetRegistration",
                "Json",
                "ToolMenus",
                "EditorWidgets"
                ,"PropertyEditor"
            }
			);

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "LauncherPlatform",
                "GameProjectGeneration",
                "ProjectTargetPlatformEditor",
                "LevelEditor",
                "Settings",
                "SourceCodeAccess",
                "HotReload",
                "AssetRegistry",
                "AssetTools",
                "ClassViewer",
                "StructViewer",
                "ContentBrowser",
            }
        );
        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

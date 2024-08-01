// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LukeToolMenu : ModuleRules
{
    public LukeToolMenu(ReadOnlyTargetRules Target) : base(Target)
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
                "ActorPlaceTool",

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

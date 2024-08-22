// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SlateFwd.h"
#include "HAL/IConsoleManager.h"
#include "Input/Reply.h"
#include "Framework/Commands/Commands.h"

class FUICommandList;
class FMenuBuilder;
class FToolBarBuilder;

class FLukeToolMenuModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// Add buttons to menus
	virtual void AddMenuEntry(FName NewEntryName, const TAttribute<FText>& InEntryLabel, const TAttribute<FText>& InEntryToolTip, const TAttribute<FSlateIcon>& InSlateIcon, FExecuteAction EntryAction, const FName SectionName = NAME_None, const TAttribute<FText>& InSectionLabel = INVTEXT(""), const FToolMenuInsert InSectionPosition = FToolMenuInsert());
private:
	void CreateMenu();
	void AddMenuEx();

	const FName LevelMenuName{ TEXT("LevelEditor.MainMenu") };
	const FName MenuName{ TEXT("LukeTools") };
};

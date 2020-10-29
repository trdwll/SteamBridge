// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamBridge.h"

#include "Developer/Settings/Public/ISettingsContainer.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Steam.h"
#include "SteamBridgeSettings.h"

#define LOCTEXT_NAMESPACE "FSteamBridgeModule"

void FSteamBridgeModule::StartupModule()
{
	RegisterSettings();
}

void FSteamBridgeModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

bool FSteamBridgeModule::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();

	return true;
}

bool FSteamBridgeModule::HandleSettingsSaved()
{
#if WITH_EDITOR
	USteamBridgeSettings* Settings = GetMutableDefault<USteamBridgeSettings>();
	bool ResaveSettings = false;

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}
#endif  // WITH_EDITOR

	return true;
}

void FSteamBridgeModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		/*ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("SteamBridge",
			LOCTEXT("RuntimeWDCategoryName", "SteamBridge"),
			LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the SteamBridge game module"));*/

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Project", "SteamBridge",
			LOCTEXT("RuntimeGeneralSettingsName", "SteamBridge"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Configure Steamworks settings."),
			GetMutableDefault<USteamBridgeSettings>());

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FSteamBridgeModule::HandleSettingsSaved);
		}
	}
#endif  // WITH_EDITOR
}

void FSteamBridgeModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Project", "SteamBridge");
	}
#endif  // WITH_EDITOR
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSteamBridgeModule, SteamBridge)

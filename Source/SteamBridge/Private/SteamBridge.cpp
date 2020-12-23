// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamBridge.h"

#include "Developer/Settings/Public/ISettingsContainer.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "Steam.h"
#include "SteamBridgeSettings.h"

#define LOCTEXT_NAMESPACE "FSteamBridgeModule"

void FSteamBridgeModule::StartupModule()
{
	RegisterSettings();

	FString SteamDir = FPaths::EngineDir() / TEXT("Binaries/ThirdParty/Steamworks") / SDK_VER;
	FString SDKPath = "";

	bool bIs64Bit = false;
#if PLATFORM_64BITS
	bIs64Bit = true;
#endif

#if PLATFORM_WINDOWS
	SteamDir = FString::Printf(TEXT("%s/%s"), *SteamDir, bIs64Bit ? *FString("Win64/") : *FString("Win32/"));
	SDKPath = FPaths::Combine(*SteamDir, FString::Printf(TEXT("steam_api%s.dll"), bIs64Bit ? *FString("64") : *FString("")));
	m_SteamLibSDKHandle = FPlatformProcess::GetDllHandle(*(SDKPath));
#elif PLATFORM_LINUX
	SteamDir = FString::Printf(TEXT("%s/%s"), *SteamDir, bIs64Bit ? *FString("x86_64-unknown-linux-gnu/") : *FString("i686-unknown-linux-gnu/"));
	SDKPath = FPaths::Combine(*SteamDir, "libsteam_api.so");
	m_SteamLibSDKHandle = FPlatformProcess::GetDllHandle(*(SDKPath));
#endif
}

void FSteamBridgeModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		SteamAPI_Shutdown();
		SteamGameServer_Shutdown();

		UnregisterSettings();

		if (m_SteamLibSDKHandle != nullptr)
		{
			FPlatformProcess::FreeDllHandle(m_SteamLibSDKHandle);
		}
	}
}

bool FSteamBridgeModule::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();
	SteamGameServer_RunCallbacks();

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

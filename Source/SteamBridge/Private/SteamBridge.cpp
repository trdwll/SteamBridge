// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include <Developer/Settings/Public/ISettingsContainer.h>
#include <Developer/Settings/Public/ISettingsModule.h>
#include <Developer/Settings/Public/ISettingsSection.h>
#include <HAL/FileManager.h>
#include <HAL/PlatformProcess.h>
#include <Interfaces/IPluginManager.h>
#include <Misc/Paths.h>
#include <Modules/ModuleManager.h>

#include "SteamBridge.h"
#include "Steam.h"
#include "SteamBridgeSettings.h"

#ifndef STEAM_SDK_INSTALLED
#error Steam SDK not located! Expected to be found in Engine/Source/ThirdParty/Steamworks/{SteamVersion}
#endif  // STEAM_SDK_INSTALLED

#define LOCTEXT_NAMESPACE "FSteamBridgeModule"

void FSteamBridgeModule::StartupModule()
{
	RegisterSettings();

	const FString STEAM_SDK_ROOT_PATH(TEXT("Binaries/ThirdParty/Steamworks"));

#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	FString STEAM_SDK_PATH = FPaths::EngineDir() / STEAM_SDK_ROOT_PATH / STEAM_SDK_VER_PATH / TEXT("Win64/");
#else
	FString STEAM_SDK_PATH = FPaths::EngineDir() / STEAM_SDK_ROOT_PATH / STEAM_SDK_VER_PATH / TEXT("Win32/");
#endif  //PLATFORM_64BITS
	FString SDKBinaries = FPaths::Combine(*STEAM_SDK_PATH, FString::Printf(TEXT("steam_api%s.dll"), PLATFORM_64BITS ? *FString("64") : *FString("")));
	SteamLibSDKHandle = FPlatformProcess::GetDllHandle(*(SDKBinaries));
#elif PLATFORM_LINUX
#if PLATFORM_64BITS
	FString STEAM_SDK_PATH = FPaths::EngineDir() / STEAM_SDK_ROOT_PATH / STEAM_SDK_VER_PATH / TEXT("x86_64-unknown-linux-gnu/");
#else
	FString STEAM_SDK_PATH = FPaths::EngineDir() / STEAM_SDK_ROOT_PATH / STEAM_SDK_VER_PATH / TEXT("i686-unknown-linux-gnu/");
#endif  //PLATFORM_64BITS
	FString SDKBinaries = FPaths::Combine(*STEAM_SDK_PATH, "libsteam_api.so");
	SteamLibSDKHandle = FPlatformProcess::GetDllHandle(*(SDKBinaries));
#elif PLATFORM_MAC
	FString STEAM_SDK_PATH = FPaths::EngineDir() / STEAM_SDK_ROOT_PATH / STEAM_SDK_VER_PATH / TEXT("Mac/");
	FString SDKBinaries = FPaths::Combine(*STEAM_SDK_PATH, "libsteam_api.dylib");
	SteamLibSDKHandle = FPlatformProcess::GetDllHandle(*(SDKBinaries));
#endif
}

void FSteamBridgeModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		SteamAPI_Shutdown();
		SteamGameServer_Shutdown();

		UnregisterSettings();

		if (SteamLibSDKHandle != nullptr)
		{
			FPlatformProcess::FreeDllHandle(SteamLibSDKHandle);
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

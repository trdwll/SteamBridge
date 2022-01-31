// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Containers/Ticker.h>
#include <Modules/ModuleManager.h>

class FSteamBridgeModule : public IModuleInterface, public FTickerObjectBase
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool Tick(float DeltaTime) override;

private:

	bool HandleSettingsSaved();
	void RegisterSettings();
	void UnregisterSettings();

	void* SteamLibSDKHandle;
};

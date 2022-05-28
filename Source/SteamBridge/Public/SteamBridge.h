// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Containers/Ticker.h>
#include <Modules/ModuleManager.h>

class FSteamBridgeModule : public IModuleInterface,
#if ENGINE_MAJOR_VERSION == 5
		public FTSTickerObjectBase
#else
		public FTickerObjectBase
#endif
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

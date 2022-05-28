// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.
#pragma once

#include <Runtime/Launch/Resources/Version.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#pragma warning(disable:4265)
#endif

#if ENGINE_MAJOR_VERSION == 5
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_gameserver.h"
#else
#include "ThirdParty/Steamworks/Steamv147/sdk/public/steam/steam_api.h"
#include "ThirdParty/Steamworks/Steamv147/sdk/public/steam/steam_gameserver.h"
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Not completely ideal to put this here, but this file is included everywhere so maybe it is?

namespace SteamDefs
{
	static constexpr int32 Buffer128 = 128;
	static constexpr int32 Buffer512 = 512;
	static constexpr int32 Buffer1024 = 1024;
	static constexpr int32 Buffer2048 = 2048;
	static constexpr int32 Buffer4096 = 4096;
	static constexpr int32 Buffer8192 = 8192;
	static constexpr int32 Buffer16384 = 16384;
};

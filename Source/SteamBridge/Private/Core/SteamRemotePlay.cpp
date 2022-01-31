// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamRemotePlay.h"

#include "SteamBridgeUtils.h"

USteamRemotePlay::USteamRemotePlay()
{
	OnSteamRemotePlaySessionConnectedCallback.Register(this, &USteamRemotePlay::OnSteamRemotePlaySessionConnected);
	OnSteamRemotePlaySessionDisconnectedCallback.Register(this, &USteamRemotePlay::OnSteamRemotePlaySessionDisconnected);
}

USteamRemotePlay::~USteamRemotePlay()
{
	OnSteamRemotePlaySessionConnectedCallback.Unregister();
	OnSteamRemotePlaySessionDisconnectedCallback.Unregister();
}

bool USteamRemotePlay::BGetSessionClientResolution(int32 SessionID, FIntPoint& Resolution) const
{
	int32 x, y;
	bool bResult = SteamRemotePlay()->BGetSessionClientResolution(SessionID, &x, &y);
	Resolution = {x,y};
	return bResult;
}

void USteamRemotePlay::OnSteamRemotePlaySessionConnected(SteamRemotePlaySessionConnected_t* pParam)
{
	OnSteamRemotePlaySessionConnectedDelegate.Broadcast(pParam->m_unSessionID);
}

void USteamRemotePlay::OnSteamRemotePlaySessionDisconnected(SteamRemotePlaySessionDisconnected_t* pParam)
{
	OnSteamRemotePlaySessionDisconnectedDelegate.Broadcast(pParam->m_unSessionID);
}

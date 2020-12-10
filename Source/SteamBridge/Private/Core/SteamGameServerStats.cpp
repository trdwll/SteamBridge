// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamGameServerStats.h"

#include "SteamBridgeUtils.h"

USteamGameServerStats::USteamGameServerStats()
{
	OnGSStatsReceivedCallback.Register(this, &USteamGameServerStats::OnGSStatsReceived);
	OnGSStatsStoredCallback.Register(this, &USteamGameServerStats::OnGSStatsStored);
	OnGSStatsUnloadedCallback.Register(this, &USteamGameServerStats::OnGSStatsUnloaded);
}

USteamGameServerStats::~USteamGameServerStats()
{
	OnGSStatsReceivedCallback.Unregister();
	OnGSStatsStoredCallback.Unregister();
	OnGSStatsUnloadedCallback.Unregister();
}

bool USteamGameServerStats::UpdateUserAvgRateStat(FSteamID SteamIDUser, const FString& Name, float CountThisSession, float SessionLength) const
{
	return SteamGameServerStats()->UpdateUserAvgRateStat(SteamIDUser, TCHAR_TO_UTF8(*Name), CountThisSession, SessionLength);
}

void USteamGameServerStats::OnGSStatsReceived(GSStatsReceived_t *pParam)
{
	m_OnGSStatsReceived.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_steamIDUser.ConvertToUint64());
}

void USteamGameServerStats::OnGSStatsStored(GSStatsStored_t *pParam)
{
	m_OnGSStatsStored.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_steamIDUser.ConvertToUint64());
}

void USteamGameServerStats::OnGSStatsUnloaded(GSStatsUnloaded_t *pParam)
{
	m_OnGSStatsUnloaded.Broadcast(pParam->m_steamIDUser.ConvertToUint64());
}

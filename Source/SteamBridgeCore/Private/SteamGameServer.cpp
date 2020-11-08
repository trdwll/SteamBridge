// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "SteamGameServer.h"

#include "SteamBridgeUtils.h"

USteamGameServer::USteamGameServer()
{
}

USteamGameServer::~USteamGameServer()
{
}

ESteamBeginAuthSessionResult USteamGameServer::BeginAuthSession(TArray<uint8> AuthTicket, FSteamID SteamID)
{
	AuthTicket.SetNum(8192);
	return (ESteamBeginAuthSessionResult)SteamGameServer()->BeginAuthSession(AuthTicket.GetData(), 8192, SteamID.Value);
}

FHAuthTicket USteamGameServer::GetAuthSessionTicket(TArray<uint8>& AuthTicket)
{
	uint32 length = 0;
	AuthTicket.SetNum(8192);
	FHAuthTicket result = (FHAuthTicket)SteamGameServer()->GetAuthSessionTicket(AuthTicket.GetData(), 8192, &length);
	AuthTicket.SetNum(length);
	return result;
}

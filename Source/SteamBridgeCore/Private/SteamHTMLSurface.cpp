// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "SteamHTMLSurface.h"

#include "SteamBridgeUtils.h"

USteamHTMLSurface::USteamHTMLSurface()
{

}

USteamHTMLSurface::~USteamHTMLSurface()
{
}

void USteamHTMLSurface::SetCookie(const FString& Hostname, const FString& Key, const FString& Value, const FString& Path, int32 Expires, bool bSecure, bool bHTTPOnly)
{
	SteamHTMLSurface()->SetCookie(TCHAR_TO_UTF8(*Hostname), TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value), TCHAR_TO_UTF8(*Path), Expires, bSecure, bHTTPOnly);
}

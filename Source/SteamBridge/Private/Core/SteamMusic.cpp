// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamMusic.h"

#include "SteamBridgeUtils.h"

USteamMusic::USteamMusic()
{
	OnPlaybackStatusHasChangedCallback.Register(this, &USteamMusic::OnPlaybackStatusHasChanged);
	OnVolumeHasChangedCallback.Register(this, &USteamMusic::OnVolumeHasChanged);
}

USteamMusic::~USteamMusic()
{
	OnPlaybackStatusHasChangedCallback.Unregister();
	OnVolumeHasChangedCallback.Unregister();
}

void USteamMusic::OnPlaybackStatusHasChanged(PlaybackStatusHasChanged_t* pParam)
{
	m_OnPlaybackStatusHasChanged.Broadcast();
}

void USteamMusic::OnVolumeHasChanged(VolumeHasChanged_t* pParam)
{
	m_OnVolumeHasChanged.Broadcast(pParam->m_flNewVolume);
}
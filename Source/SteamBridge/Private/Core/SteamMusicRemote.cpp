// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamMusicRemote.h"

#include "SteamBridgeUtils.h"

USteamMusicRemote::USteamMusicRemote()
{
	OnMusicPlayerRemoteToFrontCallback.Register(this, &USteamMusicRemote::OnMusicPlayerRemoteToFront);
	OnMusicPlayerRemoteWillActivateCallback.Register(this, &USteamMusicRemote::OnMusicPlayerRemoteWillActivate);
	OnMusicPlayerRemoteWillDeactivateCallback.Register(this, &USteamMusicRemote::OnMusicPlayerRemoteWillDeactivate);
	OnMusicPlayerSelectsPlaylistEntryCallback.Register(this, &USteamMusicRemote::OnMusicPlayerSelectsPlaylistEntry);
	OnMusicPlayerSelectsQueueEntryCallback.Register(this, &USteamMusicRemote::OnMusicPlayerSelectsQueueEntry);
	OnMusicPlayerWantsLoopedCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsLooped);
	OnMusicPlayerWantsPauseCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsPause);
	OnMusicPlayerWantsPlayingRepeatStatusCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsPlayingRepeatStatus);
	OnMusicPlayerWantsPlayNextCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsPlayNext);
	OnMusicPlayerWantsPlayPreviousCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsPlayPrevious);
	OnMusicPlayerWantsPlayCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsPlay);
	OnMusicPlayerWantsShuffledCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsShuffled);
	OnMusicPlayerWantsVolumeCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWantsVolume);
	OnMusicPlayerWillQuitCallback.Register(this, &USteamMusicRemote::OnMusicPlayerWillQuit);
}

USteamMusicRemote::~USteamMusicRemote()
{
	OnMusicPlayerRemoteToFrontCallback.Unregister();
	OnMusicPlayerRemoteWillActivateCallback.Unregister();
	OnMusicPlayerRemoteWillDeactivateCallback.Unregister();
	OnMusicPlayerSelectsPlaylistEntryCallback.Unregister();
	OnMusicPlayerSelectsQueueEntryCallback.Unregister();
	OnMusicPlayerWantsLoopedCallback.Unregister();
	OnMusicPlayerWantsPauseCallback.Unregister();
	OnMusicPlayerWantsPlayingRepeatStatusCallback.Unregister();
	OnMusicPlayerWantsPlayNextCallback.Unregister();
	OnMusicPlayerWantsPlayPreviousCallback.Unregister();
	OnMusicPlayerWantsPlayCallback.Unregister();
	OnMusicPlayerWantsShuffledCallback.Unregister();
	OnMusicPlayerWantsVolumeCallback.Unregister();
	OnMusicPlayerWillQuitCallback.Unregister();
}

void USteamMusicRemote::OnMusicPlayerRemoteToFront(MusicPlayerRemoteToFront_t* pParam)
{
	m_OnMusicPlayerRemoteToFront.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerRemoteWillActivate(MusicPlayerRemoteWillActivate_t* pParam)
{
	m_OnMusicPlayerRemoteWillActivate.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerRemoteWillDeactivate(MusicPlayerRemoteWillDeactivate_t* pParam)
{
	m_OnMusicPlayerRemoteWillDeactivate.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerSelectsPlaylistEntry(MusicPlayerSelectsPlaylistEntry_t* pParam)
{
	m_OnMusicPlayerSelectsPlaylistEntry.Broadcast(pParam->nID);
}

void USteamMusicRemote::OnMusicPlayerSelectsQueueEntry(MusicPlayerSelectsQueueEntry_t* pParam)
{
	m_OnMusicPlayerSelectsQueueEntry.Broadcast(pParam->nID);
}

void USteamMusicRemote::OnMusicPlayerWantsLooped(MusicPlayerWantsLooped_t* pParam)
{
	m_OnMusicPlayerWantsLooped.Broadcast(pParam->m_bLooped);
}

void USteamMusicRemote::OnMusicPlayerWantsPause(MusicPlayerWantsPause_t* pParam)
{
	m_OnMusicPlayerWantsPause.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerWantsPlayingRepeatStatus(MusicPlayerWantsPlayingRepeatStatus_t* pParam)
{
	m_OnMusicPlayerWantsPlayingRepeatStatus.Broadcast(pParam->m_nPlayingRepeatStatus);
}

void USteamMusicRemote::OnMusicPlayerWantsPlayNext(MusicPlayerWantsPlayNext_t* pParam)
{
	m_OnMusicPlayerWantsPlayNext.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerWantsPlayPrevious(MusicPlayerWantsPlayPrevious_t* pParam)
{
	m_OnMusicPlayerWantsPlayPrevious.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerWantsPlay(MusicPlayerWantsPlay_t* pParam)
{
	m_OnMusicPlayerWantsPlay.Broadcast();
}

void USteamMusicRemote::OnMusicPlayerWantsShuffled(MusicPlayerWantsShuffled_t* pParam)
{
	m_OnMusicPlayerWantsShuffled.Broadcast(pParam->m_bShuffled);
}

void USteamMusicRemote::OnMusicPlayerWantsVolume(MusicPlayerWantsVolume_t* pParam)
{
	m_OnMusicPlayerWantsVolume.Broadcast(pParam->m_flNewVolume);
}

void USteamMusicRemote::OnMusicPlayerWillQuit(MusicPlayerWillQuit_t* pParam)
{
	m_OnMusicPlayerWillQuit.Broadcast();
}

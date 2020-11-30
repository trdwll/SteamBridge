// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamMusicRemote.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerRemoteToFrontDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerRemoteWillActivateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerRemoteWillDeactivateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerSelectsPlaylistEntryDelegate, int32, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerSelectsQueueEntryDelegate, int32, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerWantsLoopedDelegate, bool, bLooped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerWantsPauseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerWantsPlayingRepeatStatusDelegate, int32, RepeatStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerWantsPlayNextDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerWantsPlayPreviousDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerWantsPlayDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerWantsShuffledDelegate, bool, bShuffled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicPlayerWantsVolumeDelegate, float, NewVolume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicPlayerWillQuitDelegate);

class UTexture2D;

/**
 * Allows direct interaction with the Steam Music player.
 * https://partner.steamgames.com/doc/api/ISteamMusicRemote
 */
UCLASS()
class STEAMBRIDGE_API USteamMusicRemote final : public UObject
{
	GENERATED_BODY()

public:
	USteamMusicRemote();
	~USteamMusicRemote();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Music Remote", CompactNodeTitle = "SteamMusicRemote"))
	static USteamMusicRemote* GetSteamMusicRemote() { return USteamMusicRemote::StaticClass()->GetDefaultObject<USteamMusicRemote>(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool BActivationSuccess(bool bValue) const { return SteamMusicRemote()->BActivationSuccess(bValue); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool BIsCurrentMusicRemote() const { return SteamMusicRemote()->BIsCurrentMusicRemote(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool CurrentEntryIsAvailable(bool bAvailable) const { return SteamMusicRemote()->CurrentEntryIsAvailable(bAvailable); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool CurrentEntryWillChange() const { return SteamMusicRemote()->CurrentEntryWillChange(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool DeregisterSteamMusicRemote() const { return SteamMusicRemote()->DeregisterSteamMusicRemote(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnableLooped(bool bValue) const { return SteamMusicRemote()->EnableLooped(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnablePlaylists(bool bValue) const { return SteamMusicRemote()->EnablePlaylists(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnablePlayNext(bool bValue) const { return SteamMusicRemote()->EnablePlayNext(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnablePlayPrevious(bool bValue) const { return SteamMusicRemote()->EnablePlayPrevious(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnableQueue(bool bValue) const { return SteamMusicRemote()->EnableQueue(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool EnableShuffled(bool bValue) const { return SteamMusicRemote()->EnableShuffled(bValue); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool PlaylistDidChange() const { return SteamMusicRemote()->PlaylistDidChange(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool PlaylistWillChange() const { return SteamMusicRemote()->PlaylistWillChange(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool QueueDidChange() const { return SteamMusicRemote()->QueueDidChange(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MusicRemote")
	bool QueueWillChange() const { return SteamMusicRemote()->QueueWillChange(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool RegisterSteamMusicRemote(const FString& Name) const { return SteamMusicRemote()->RegisterSteamMusicRemote(TCHAR_TO_UTF8(*Name)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool ResetPlaylistEntries() const { return SteamMusicRemote()->ResetPlaylistEntries(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool ResetQueueEntries() const { return SteamMusicRemote()->ResetQueueEntries(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool SetCurrentPlaylistEntry(int32 ID) const { return SteamMusicRemote()->SetCurrentPlaylistEntry(ID); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool SetCurrentQueueEntry(int32 ID) const { return SteamMusicRemote()->SetCurrentQueueEntry(ID); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool SetDisplayName(const FString& DisplayName) const { return SteamMusicRemote()->SetDisplayName(TCHAR_TO_UTF8(*DisplayName)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool SetPlaylistEntry(int32 ID, int32 Position, const FString& EntryText) const { return SteamMusicRemote()->SetPlaylistEntry(ID, Position, TCHAR_TO_UTF8(*EntryText)); }

	// #TODO: SetPNGIcon_64x64

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool SetQueueEntry(int32 ID, int32 Position, const FString& EntryText) const { return SteamMusicRemote()->SetQueueEntry(ID, Position, TCHAR_TO_UTF8(*EntryText)); }

	// #TODO: UpdateCurrentEntryCoverArt

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdateCurrentEntryElapsedSeconds(int32 Value) const { return SteamMusicRemote()->UpdateCurrentEntryElapsedSeconds(Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdateCurrentEntryText(const FString& Text) const { return SteamMusicRemote()->UpdateCurrentEntryText(TCHAR_TO_UTF8(*Text)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdateLooped(bool bValue) const { return SteamMusicRemote()->UpdateLooped(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdatePlaybackStatus(ESteamAudioPlaybackStatus Status) const { return SteamMusicRemote()->UpdatePlaybackStatus((AudioPlayback_Status)Status); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdateShuffled(bool bValue) const { return SteamMusicRemote()->UpdateShuffled(bValue); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|MusicRemote")
	bool UpdateVolume(float Value) const { return SteamMusicRemote()->UpdateVolume(Value); }

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerRemoteToFront"))
	FOnMusicPlayerRemoteToFrontDelegate m_OnMusicPlayerRemoteToFront;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerRemoteWillActivate"))
	FOnMusicPlayerRemoteWillActivateDelegate m_OnMusicPlayerRemoteWillActivate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerRemoteWillDeactivate"))
	FOnMusicPlayerRemoteWillDeactivateDelegate m_OnMusicPlayerRemoteWillDeactivate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerSelectsPlaylistEntry"))
	FOnMusicPlayerSelectsPlaylistEntryDelegate m_OnMusicPlayerSelectsPlaylistEntry;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerSelectsQueueEntry"))
	FOnMusicPlayerSelectsQueueEntryDelegate m_OnMusicPlayerSelectsQueueEntry;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsLooped"))
	FOnMusicPlayerWantsLoopedDelegate m_OnMusicPlayerWantsLooped;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsPause"))
	FOnMusicPlayerWantsPauseDelegate m_OnMusicPlayerWantsPause;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsPlayingRepeatStatus"))
	FOnMusicPlayerWantsPlayingRepeatStatusDelegate m_OnMusicPlayerWantsPlayingRepeatStatus;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsPlayNext"))
	FOnMusicPlayerWantsPlayNextDelegate m_OnMusicPlayerWantsPlayNext;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsPlayPrevious"))
	FOnMusicPlayerWantsPlayPreviousDelegate m_OnMusicPlayerWantsPlayPrevious;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsPlay"))
	FOnMusicPlayerWantsPlayDelegate m_OnMusicPlayerWantsPlay;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsShuffled"))
	FOnMusicPlayerWantsShuffledDelegate m_OnMusicPlayerWantsShuffled;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWantsVolume"))
	FOnMusicPlayerWantsVolumeDelegate m_OnMusicPlayerWantsVolume;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|MusicRemote", meta = (DisplayName = "OnMusicPlayerWillQuit"))
	FOnMusicPlayerWillQuitDelegate m_OnMusicPlayerWillQuit;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerRemoteToFront, MusicPlayerRemoteToFront_t, OnMusicPlayerRemoteToFrontCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerRemoteWillActivate, MusicPlayerRemoteWillActivate_t, OnMusicPlayerRemoteWillActivateCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerRemoteWillDeactivate, MusicPlayerRemoteWillDeactivate_t, OnMusicPlayerRemoteWillDeactivateCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerSelectsPlaylistEntry, MusicPlayerSelectsPlaylistEntry_t, OnMusicPlayerSelectsPlaylistEntryCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerSelectsQueueEntry, MusicPlayerSelectsQueueEntry_t, OnMusicPlayerSelectsQueueEntryCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsLooped, MusicPlayerWantsLooped_t, OnMusicPlayerWantsLoopedCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsPause, MusicPlayerWantsPause_t, OnMusicPlayerWantsPauseCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsPlayingRepeatStatus, MusicPlayerWantsPlayingRepeatStatus_t, OnMusicPlayerWantsPlayingRepeatStatusCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsPlayNext, MusicPlayerWantsPlayNext_t, OnMusicPlayerWantsPlayNextCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsPlayPrevious, MusicPlayerWantsPlayPrevious_t, OnMusicPlayerWantsPlayPreviousCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsPlay, MusicPlayerWantsPlay_t, OnMusicPlayerWantsPlayCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsShuffled, MusicPlayerWantsShuffled_t, OnMusicPlayerWantsShuffledCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWantsVolume, MusicPlayerWantsVolume_t, OnMusicPlayerWantsVolumeCallback);
	STEAM_CALLBACK_MANUAL(USteamMusicRemote, OnMusicPlayerWillQuit, MusicPlayerWillQuit_t, OnMusicPlayerWillQuitCallback);
};

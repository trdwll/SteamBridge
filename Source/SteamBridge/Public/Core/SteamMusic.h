// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamMusic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaybackStatusHasChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVolumeHasChangedDelegate, float, NewVolume);

/**
 * Functions to control music playback in the steam client.
 * This gives games the opportunity to do things like pause the music or lower the volume, when an important cut scene is shown, and start playing afterwards.
 * https://partner.steamgames.com/doc/api/ISteamMusic
 */
UCLASS()
class STEAMBRIDGE_API USteamMusic final : public UObject
{
	GENERATED_BODY()

public:
	USteamMusic();
	~USteamMusic();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Music", CompactNodeTitle = "SteamMusic"))
	static USteamMusic* GetSteamMusic() { return USteamMusic::StaticClass()->GetDefaultObject<USteamMusic>(); }

	/**
	 * Checks if Steam Music is enabled.
	 *
	 * @return bool - true if it is enabled; otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Music")
	bool BIsEnabled() const { return SteamMusic()->BIsEnabled(); }

	/**
	 * Checks if Steam Music is active. This does not necessarily a song is currently playing, it may be paused.
	 * For finer grain control use GetPlaybackStatus.
	 *
	 * @return bool - true if a song is currently playing, paused, or queued up to play; otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Music")
	bool BIsPlaying() const { return SteamMusic()->BIsPlaying(); }

	/**
	 * Gets the current status of the Steam Music player.
	 *
	 * @return ESteamAudioPlaybackStatus
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Music")
	ESteamAudioPlaybackStatus GetPlaybackStatus() const { return (ESteamAudioPlaybackStatus)SteamMusic()->GetPlaybackStatus(); }

	/**
	 * Gets the current volume of the Steam Music player.
	 *
	 * @return float - The volume is returned as a percentage between 0.0 and 1.0.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Music")
	float GetVolume() const { return SteamMusic()->GetVolume(); }

	/**
	 * Pause the Steam Music player.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Music")
	void Pause() { SteamMusic()->Pause(); }

	/**
	 * Have the Steam Music player resume playing.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Music")
	void Play() { SteamMusic()->Play(); }

	/**
	 * Have the Steam Music player skip to the next song.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Music")
	void PlayNext() { SteamMusic()->PlayNext(); }

	/**
	 * Have the Steam Music player play the previous song.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Music")
	void PlayPrevious() { SteamMusic()->PlayPrevious(); }

	/**
	 * Sets the volume of the Steam Music player.
	 *
	 * @param float Volume - The volume percentage to set from 0.0 to 1.0.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Music")
	void SetVolume(float Volume) { SteamMusic()->SetVolume(Volume); }

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Music", meta = (DisplayName = "OnPlaybackStatusHasChanged"))
	FOnPlaybackStatusHasChangedDelegate OnPlaybackStatusHasChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Music", meta = (DisplayName = "OnVolumeHasChanged"))
	FOnVolumeHasChangedDelegate OnVolumeHasChangedDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamMusic, OnPlaybackStatusHasChanged, PlaybackStatusHasChanged_t, OnPlaybackStatusHasChangedCallback);
	STEAM_CALLBACK_MANUAL(USteamMusic, OnVolumeHasChanged, VolumeHasChanged_t, OnVolumeHasChangedCallback);
};

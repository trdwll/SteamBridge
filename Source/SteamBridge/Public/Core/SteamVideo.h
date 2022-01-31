// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamVideo.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBroadcastUploadStartDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBroadcastUploadStopDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetOPFSettingsResultDelegate, ESteamResult, Result, int32, VideoAppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGetVideoURLResultDelegate, ESteamResult, Result, int32, VideoAppID, FString, URL);

/**
 * Provides functions to interface with the Steam video and broadcasting platforms.
 * https://partner.steamgames.com/doc/api/ISteamVideo
 */
UCLASS()
class STEAMBRIDGE_API USteamVideo final : public UObject
{
	GENERATED_BODY()

public:
	USteamVideo();
	~USteamVideo();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Video", CompactNodeTitle = "SteamVideo"))
	static USteamVideo* GetSteamVideo() { return USteamVideo::StaticClass()->GetDefaultObject<USteamVideo>(); }

	/**
	 * Get the OPF details for 360 video playback
	 * To retrieve the 360 OPF (open projection format) data to playback a 360 video, start by making a call to this, then the callback will indicate whether the request was successful. If it was successful, the
	 * actual OPF JSON data can be retrieved with a call to GetOPFStringForApp.
	 * Triggers a GetOPFSettingsResult_t callback.
	 *
	 * @param int32 VideoAppID - The video app ID to get the OPF details of.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Video")
	void GetOPFSettings(int32 VideoAppID) { SteamVideo()->GetOPFSettings(VideoAppID); }

	/**
	 * Gets the OPF string for the specified video App ID.
	 * Once the callback for GetOPFSettingsResult_t has been raised and the EResult indicates success, then calling this will return back the actual OPF data in a JSON format. The size of the OPF string varies, but at
	 * this time 48,000 bytes should be sufficient to contain the full string. If it is not, pnBufferSize will be set to the size required. In that case, make a second call with the correct buffer size.
	 * NOTE: The data returned in a successful call to GetOPFStringForApp() can only be retrieved once. If you need to retrieve it multiple times, a call to GetOPFSettings will need to be made each time.
	 *
	 * @param int32 VideoAppID - The video app ID to get the OPF string for.
	 * @param FString & OPFString - Returns the OPF string by writing it to this buffer.
	 * @return bool - true if we have the OPF details from a previous call to GetOPFSettings, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Video")
	bool GetOPFStringForApp(int32 VideoAppID, FString& OPFString) const;

	/**
	 * Asynchronously gets the URL suitable for streaming the video associated with the specified video app ID.
	 * Triggers a GetVideoURLResult_t callback.
	 *
	 * @param int32 VideoAppID - The video app ID to receive the video stream for.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Video")
	void GetVideoURL(int32 VideoAppID) { SteamVideo()->GetVideoURL(VideoAppID); }

	/**
	 * Checks if the user is currently live broadcasting and gets the number of users.
	 *
	 * @param int32 & NumViewers - Returns the number of viewers currently watching the live broadcast.
	 * @return bool - true if user is uploading a live broadcast, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Video")
	bool IsBroadcasting(int32& NumViewers) { return SteamVideo()->IsBroadcasting(&NumViewers); }

	/** Delegates */

	/*UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Video", meta = (DisplayName = "OnBroadcastUploadStart"))
	FOnBroadcastUploadStartDelegate m_OnBroadcastUploadStart;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Video", meta = (DisplayName = "OnBroadcastUploadStop"))
	FOnBroadcastUploadStopDelegate m_OnBroadcastUploadStop;*/

	/** Triggered when the OPF Details for 360 video playback are retrieved. After receiving this you can use GetOPFStringForApp to access the OPF details. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Video", meta = (DisplayName = "OnGetOPFSettingsResult"))
	FOnGetOPFSettingsResultDelegate OnGetOPFSettingsResultDelegate;

	/** Provides the result of a call to GetVideoURL. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Video", meta = (DisplayName = "OnGetVideoURLResult"))
	FOnGetVideoURLResultDelegate OnGetVideoURLResultDelegate;

protected:
private:
	/*STEAM_CALLBACK_MANUAL(USteamVideo, OnBroadcastUploadStart, BroadcastUploadStart_t, OnBroadcastUploadStartCallback);
	STEAM_CALLBACK_MANUAL(USteamVideo, OnBroadcastUploadStop, BroadcastUploadStop_t, OnBroadcastUploadStopCallback);*/
	STEAM_CALLBACK_MANUAL(USteamVideo, OnGetOPFSettingsResult, GetOPFSettingsResult_t, OnGetOPFSettingsResultCallback);
	STEAM_CALLBACK_MANUAL(USteamVideo, OnGetVideoURLResult, GetVideoURLResult_t, OnGetVideoURLResultCallback);
};

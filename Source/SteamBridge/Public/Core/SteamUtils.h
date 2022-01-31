// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamUtils.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGamepadTextInputDismissedDelegate, bool, bSubmitted, int32, SubmittedTextLen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIPCountryDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLowBatteryPowerDelegate, uint8, MinutesBatteryLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSteamAPICallCompletedDelegate, FSteamAPICall, APICallHandle, int32, CallbackID, int32, CallbackSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSteamShutdownDelegate);

/**
 * Interface which provides access to a range of miscellaneous utility functions.
 * https://partner.steamgames.com/doc/api/ISteamUtils
 */
UCLASS()
class STEAMBRIDGE_API USteamUtils final : public UObject
{
	GENERATED_BODY()

public:
	USteamUtils();
	~USteamUtils();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Utils", CompactNodeTitle = "SteamUtils"))
	static USteamUtils* GetSteamUtils() { return USteamUtils::StaticClass()->GetDefaultObject<USteamUtils>(); }

	/**
	 * Checks if the Overlay needs a present. Only required if using event driven render updates.
	 * Typically this call is unneeded if your game has a constantly running frame loop that calls the D3D Present API, or OGL SwapBuffers API every frame as is the case in most games. However, if you have a game that
	 * only refreshes the screen on an event driven basis then that can break the overlay, as it uses your Present/SwapBuffers calls to drive it's internal frame loop and it may also need to Present() to the screen any
	 * time a notification happens or when the overlay is brought up over the game by a user. You can use this API to ask the overlay if it currently need a present in that case, and then you can check for this
	 * periodically (roughly 33hz is desirable) and make sure you refresh the screen with Present or SwapBuffers to allow the overlay to do it's work.
	 *
	 * @return bool - true if the overlay needs you to refresh the screen, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool BOverlayNeedsPresent() const { return SteamUtils()->BOverlayNeedsPresent(); }

	/**
	 * Used to get the failure reason of a call result.
	 * The primary usage for this function is debugging. The failure reasons are typically out of your control and tend to not be very important. Just keep retrying your API Call until it works.
	 *
	 * @param FSteamAPICall SteamAPICall - 	The Steam API Call handle to check the failure for.
	 * @return ESteamAPICallFailure_
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	ESteamAPICallFailure_ GetAPICallFailureReason(FSteamAPICall SteamAPICall) const { return (ESteamAPICallFailure_)((uint8)SteamUtils()->GetAPICallFailureReason(SteamAPICall) - 1); }

	// #TODO: GetAPICallResult

	/**
	 * Gets the App ID of the current process.
	 *
	 * @return int32 - Returns the AppId.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	int32 GetAppID() const { return SteamUtils()->GetAppID(); }

	/**
	 * Gets the universe that the current client is connecting to. (Valve use only.)
	 *
	 * @return ESteamUniverse
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	ESteamUniverse GetConnectedUniverse() const { return (ESteamUniverse)SteamUtils()->GetConnectedUniverse(); }

	/**
	 * Gets the IP of the reporting server for valve - currently only used in Source engine games
	 *
	 * @param FString & IP - Returns the IP that the client is connected to.
	 * @param int32 & Port - Returns the port that the client is connected to.
	 * @return bool - Returns true if the user is currently connected; otherwise, false.
	 */
	// GetCSERIPPort is deprecated as of more recent Steamworks SDK releases, so this has been dummied out.
	//UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Utils")
	//bool GetCSERIPPort(FString& IP, int32& Port) const;

	/**
	 * Gets the current amount of battery power on the computer.
	 *
	 * @return uint8 - The current battery power ranging between [0..100]%. Returns 255 when the user is on AC power.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	uint8 GetCurrentBatteryPower() const { return SteamUtils()->GetCurrentBatteryPower(); }

	/**
	 * Gets the gamepad text input from the Big Picture overlay.
	 * This must be called within the GamepadTextInputDismissed_t callback, and only if GamepadTextInputDismissed_t.m_bSubmitted is true.
	 * Provides the text input as UTF8.
	 *
	 * @param FString & Text - DESCHERE
	 * @return bool - true if there was text to receive and cchText is the same size as GamepadTextInputDismissed_t.m_unSubmittedText; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Utils")
	bool GetEnteredGamepadTextInput(FString& Text) const;

	/**
	 * Gets the length of the gamepad text input from the Big Picture overlay.
	 * This must be called within the GamepadTextInputDismissed_t callback, and only if GamepadTextInputDismissed_t.m_bSubmitted is true.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	int32 GetEnteredGamepadTextLength() const { return SteamUtils()->GetEnteredGamepadTextLength(); }

	/**
	 * Gets the image bytes from an image handle.
	 * Prior to calling this you must get the size of the image by calling GetImageSize so that you can create your buffer with an appropriate size. You can then allocate your buffer with the width and height as:
	 * width * height * 4. The image is provided in RGBA format. This call can be somewhat expensive as it converts from the compressed type (JPG, PNG, TGA) and provides no internal caching of returned buffer,
	 * thus it is highly recommended to only call this once per image handle and cache the result. This function is only used for Steam Avatars and Achievement images and those are not expected to change mid game.
	 *
	 * @param int32 Image - The handle to the image that will be obtained.
	 * @param TArray<uint8> & Buffer - The buffer that will be filled.
	 * @return bool - true upon success if the image handle is valid and the buffer was filled out, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool GetImageRGBA(int32 Image, TArray<uint8>& Buffer) const;

	/**
	 * Gets the size of a Steam image handle.
	 * This must be called before calling GetImageRGBA to create an appropriately sized buffer that will be filled with the raw image data.
	 *
	 * @param int32 Image - The image handle to get the size for.
	 * @param FIntPoint & Size - Returns the width/height of the image.
	 * @return bool - true upon success if the image handle is valid and the sizes were filled out, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool GetImageSize(int32 Image, FIntPoint& Size) const;

	/**
	 * Returns the number of IPC calls made since the last time this function was called.
	 * Used for perf debugging so you can determine how many IPC (Inter-Process Communication) calls your game makes per frame
	 * Every IPC call is at minimum a thread context switch if not a process one so you want to rate control how often you do them.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	int32 GetIPCCallCount() const { return SteamUtils()->GetIPCCallCount(); }

	/**
	 * Returns the 2 digit ISO 3166-1-alpha-2 format country code which client is running in. e.g "US" or "UK".
	 * This is looked up via an IP-to-location database.
	 *
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	FString GetIPCountry() const { return UTF8_TO_TCHAR(SteamUtils()->GetIPCountry()); }

	/**
	 * Returns the number of seconds since the application was active.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	int32 GetSecondsSinceAppActive() const { return SteamUtils()->GetSecondsSinceAppActive(); }

	/**
	 * Returns the number of seconds since the user last moved the mouse.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	int32 GetSecondsSinceComputerActive() const { return SteamUtils()->GetSecondsSinceComputerActive(); }

	/**
	 * Returns the Steam server time in a friendly format.
	 *
	 * @return FDateTime
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	FDateTime GetServerRealTime() const { return FDateTime::FromUnixTimestamp(SteamUtils()->GetServerRealTime()); }

	/**
	 * Returns the language the steam client is running in.
	 * You probably want ISteamApps::GetCurrentGameLanguage instead, this should only be used in very special cases.
	 *
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	FString GetSteamUILanguage() const { return UTF8_TO_TCHAR(SteamUtils()->GetSteamUILanguage()); }

	/**
	 * Checks if an API Call is completed. Provides the backend of the CallResult wrapper.
	 * It's generally not recommended that you use this yourself.
	 *
	 * @param FSteamAPICall SteamAPICall - The API Call handle to check.
	 * @param bool & bFailed - Returns whether the API call has encountered a failure (true) or not (false).
	 * @return bool - true if the API Call is valid and has completed, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsAPICallCompleted(FSteamAPICall SteamAPICall, bool& bFailed) const { return SteamUtils()->IsAPICallCompleted(SteamAPICall, &bFailed); }

	/**
	 * Checks if the Steam Overlay is running & the user can access it.
	 * The overlay process could take a few seconds to start & hook the game process, so this function will initially return false while the overlay is loading.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsOverlayEnabled() const { return SteamUtils()->IsOverlayEnabled(); }

	/**
	 * Returns whether the current launcher is a Steam China launcher. You can cause the client to behave as the Steam China launcher by adding -dev -steamchina to the command line when running Steam.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsSteamChinaLauncher() const { return SteamUtils()->IsSteamChinaLauncher(); }

	/**
	 * Initializes text filtering, loading dictionaries for the language the game is running in.
	 *
	 * @return bool - true if initialization succeeds, false if filtering is unavailable for the game's language, in which case FilterText() will act as a passthrough.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Utils")
	bool InitFilterText() const { return SteamUtils()->InitFilterText(); }

	// #NOTE: There's a conflict with API and docs
	// #TODO: FilterText

	/**
	 * Checks if Steam & the Steam Overlay are running in Big Picture mode.
	 * Games must be launched through the Steam client to enable the Big Picture overlay.
	 * During development, a game can be added as a non-steam game to the developers library to test this feature.
	 *
	 * @return bool - true if the Big Picture overlay is available; otherwise, false. This will always return false if your app is not the 'game' application type.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsSteamInBigPictureMode() const { return SteamUtils()->IsSteamInBigPictureMode(); }

	/**
	 * Checks if Steam is running in VR mode.
	 *
	 * @return bool - true if Steam itself is running in VR mode; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsSteamRunningInVR() const { return SteamUtils()->IsSteamRunningInVR(); }

	/**
	 * Checks if the HMD view will be streamed via Steam Remote Play.
	 *
	 * @return bool - true if VR is enabled and the HMD view is currently being streamed; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Utils")
	bool IsVRHeadsetStreamingEnabled() const { return SteamUtils()->IsVRHeadsetStreamingEnabled(); }

	/**
	 * Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
	 * A value of (0, 0) resets the position into the corner.
	 * This position is per-game and is reset each launch.
	 *
	 * @param const FIntPoint & Inset - The horizontal (left-right) and vertical (up-down) distance in pixels from the corner.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Utils")
	void SetOverlayNotificationInset(const FIntPoint& Inset) { SteamUtils()->SetOverlayNotificationInset(Inset.X, Inset.Y); }

	/**
	 * Sets which corner the Steam overlay notification popup should display itself in.
	 * You can also set the distance from the specified corner by using SetOverlayNotificationInset.
	 * This position is per-game and is reset each launch.
	 *
	 * @param ESteamNotificationPosition NotificationPosition
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Utils")
	void SetOverlayNotificationPosition(ESteamNotificationPosition NotificationPosition) { SteamUtils()->SetOverlayNotificationPosition((ENotificationPosition)NotificationPosition); }

	/**
	 * Set whether the HMD content will be streamed via Steam Remote Play.
	 * If this is enabled, then the scene in the HMD headset will be streamed, and remote input will not be allowed. Otherwise if this is disabled, then the application window will be streamed instead, and remote input
	 * will be allowed. VR games default to enabled unless "VRHeadsetStreaming" "0" is in the extended appinfo for a game.
	 * This is useful for games that have asymmetric multiplayer gameplay.
	 *
	 * @param bool bEnabled - Turns VR HMD Streaming on (true) or off (false).
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Utils")
	void SetVRHeadsetStreamingEnabled(bool bEnabled) { SteamUtils()->SetVRHeadsetStreamingEnabled(bEnabled); }

	// #TODO: SetWarningMessageHook

	/**
	 * Activates the Big Picture text input dialog which only supports gamepad input.
	 *
	 * @param ESteamGamepadTextInputMode InputMode - Selects the input mode to use, either Normal or Password (hidden text)
	 * @param ESteamGamepadTextInputLineMode LineInputMode - Controls whether to use single or multi line input.
	 * @param const FString & Description - Sets the description that should inform the user what the input dialog is for.
	 * @param const FString & ExistingText - Sets the preexisting text which the user can edit.
	 * @param int32 CharMax - The maximum number of characters that the user can input.
	 * @return bool - true if the big picture overlay is running; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Utils")
	bool ShowGamepadTextInput(ESteamGamepadTextInputMode InputMode, ESteamGamepadTextInputLineMode LineInputMode, const FString& Description, const FString& ExistingText, int32 CharMax = 200) const;

	/**
	 * Asks Steam to create and render the OpenVR dashboard.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Utils")
	void StartVRDashboard() { SteamUtils()->StartVRDashboard(); }

	/** Delegates */

	/** Called when the big picture gamepad text input has been closed. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Utils", meta = (DisplayName = "OnGamepadTextInputDismissed"))
	FOnGamepadTextInputDismissedDelegate OnGamepadTextInputDismissedDelegate;

	/** Called when the country of the user changed. The country should be updated with GetIPCountry. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Utils", meta = (DisplayName = "OnIPCountry"))
	FOnIPCountryDelegate OnIPCountryDelegate;

	/** Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Utils", meta = (DisplayName = "OnLowBatteryPower"))
	FOnLowBatteryPowerDelegate OnLowBatteryPowerDelegate;

	/** Called when a SteamAPICall_t has completed (or failed) */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Utils", meta = (DisplayName = "OnSteamAPICallCompleted"))
	FOnSteamAPICallCompletedDelegate OnSteamAPICallCompletedDelegate;

	/** Called when Steam wants to shutdown. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Utils", meta = (DisplayName = "OnSteamShutdown"))
	FOnSteamShutdownDelegate OnSteamShutdownDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamUtils, OnGamepadTextInputDismissed, GamepadTextInputDismissed_t, OnGamepadTextInputDismissedCallback);
	STEAM_CALLBACK_MANUAL(USteamUtils, OnIPCountry, IPCountry_t, OnIPCountryCallback);
	STEAM_CALLBACK_MANUAL(USteamUtils, OnLowBatteryPower, LowBatteryPower_t, OnLowBatteryPowerCallback);
	STEAM_CALLBACK_MANUAL(USteamUtils, OnSteamAPICallCompleted, SteamAPICallCompleted_t, OnSteamAPICallCompletedCallback);
	STEAM_CALLBACK_MANUAL(USteamUtils, OnSteamShutdown, SteamShutdown_t, OnSteamShutdownCallback);
};

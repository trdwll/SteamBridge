// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamScreenshots.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenshotReadyDelegate, FScreenshotHandle, ScreenshotHandle, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenshotRequestedDelegate);

/**
 * Functions for adding screenshots to the user's screenshot library.
 * https://partner.steamgames.com/doc/api/ISteamScreenshots
 */
UCLASS()
class STEAMBRIDGE_API USteamScreenshots final : public UObject
{
	GENERATED_BODY()

public:
	USteamScreenshots();
	~USteamScreenshots();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Screenshots", CompactNodeTitle = "SteamScreenshots"))
	static USteamScreenshots* GetSteamScreenshots() { return USteamScreenshots::StaticClass()->GetDefaultObject<USteamScreenshots>(); }

	/**
	 * Adds a screenshot to the user's Steam screenshot library from disk.
	 * Triggers a ScreenshotReady_t callback.
	 * This call is asynchronous, a ScreenshotReady_t callback will be sent when the screenshot has finished writing to disk.
	 *
	 * @param const FString & FileName - The absolute file path to the JPG, PNG, or TGA screenshot.
	 * @param const FString & ThumbnailFileName - The absolute file path to an optional thumbnail image. This must be 200px wide, as described by k_ScreenshotThumbWidth and the same aspect ratio. Pass NULL if there is no thumbnail, one will be created automatically.
	 * @param const FIntPoint & Size - The size of the screenshot.
	 * @return FScreenshotHandle - The handle to this screenshot that is valid for the duration of the game process and can be used to apply tags. Returns INVALID_SCREENSHOT_HANDLE if the file could not be saved.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	FScreenshotHandle AddScreenshotToLibrary(const FString& FileName, const FString& ThumbnailFileName, const FIntPoint& Size) const;

	/**
	 * Adds a VR screenshot to the user's Steam screenshot library from disk in the supported type.
	 * Triggers a ScreenshotReady_t callback.
	 * This call is asynchronous, a ScreenshotReady_t callback will be sent when the screenshot has finished writing to disk.
	 *
	 * @param ESteamVRScreenshotType Type - The type of VR screenshot that this is.
	 * @param const FString & FileName - The absolute file path to a 2D JPG, PNG, or TGA version of the screenshot for the library view.
	 * @param const FString & VRFileName - The absolute file path to the VR screenshot, this should be the same type of screenshot specified in eType.
	 * @return FScreenshotHandle - The handle to this screenshot that is valid for the duration of the game process and can be used to apply tags. Returns INVALID_SCREENSHOT_HANDLE if the file could not be saved.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	FScreenshotHandle AddVRScreenshotToLibrary(const ESteamVRScreenshotType Type, const FString& FileName, const FString& VRFileName) const;

	/**
	 * Toggles whether the overlay handles screenshots when the user presses the screenshot hotkey, or if the game handles them.
	 * Hooking is disabled by default, and only ever enabled if you do so with this function.
	 * If the hooking is enabled, then the ScreenshotRequested_t callback will be sent if the user presses the hotkey or when TriggerScreenshot is called, and then the game is expected to call WriteScreenshot or AddScreenshotToLibrary in response.
	 * You can check if hooking is enabled with IsScreenshotsHooked.
	 *
	 * @param bool bHook - Enable (true) or disable (false) hooking?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Screenshots")
	void HookScreenshots(const bool bHook) { SteamScreenshots()->HookScreenshots(bHook); }

	/**
	 * Checks if the app is hooking screenshots, or if the Steam Overlay is handling them. This can be toggled with HookScreenshots.
	 *
	 * @return bool - true if the game is hooking screenshots and is expected to handle them; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Screenshots")
	bool IsScreenshotsHooked() const { return SteamScreenshots()->IsScreenshotsHooked(); }

	/**
	 * Sets optional metadata about a screenshot's location. For example, the name of the map it was taken on.
	 * You can get the handle to tag the screenshot once it has been successfully saved from the ScreenshotReady_t callback or via the WriteScreenshot, AddScreenshotToLibrary, AddVRScreenshotToLibrary calls.
	 *
	 * @param FScreenshotHandle ScreenshotHandle - The handle to the screenshot to tag.
	 * @param const FString & Location - The location in the game where this screenshot was taken. This can not be longer than k_cubUFSTagValueMax.
	 * @return bool - true if the location was successfully added to the screenshot. false if hScreenshot was invalid, or pchLocation is invalid or too long.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	bool SetLocation(const FScreenshotHandle ScreenshotHandle, const FString& Location) const { return SteamScreenshots()->SetLocation(ScreenshotHandle, TCHAR_TO_UTF8(*Location)); }

	/**
	 * Tags a published file as being visible in the screenshot.
	 * You can tag up to the value declared by k_nScreenshotMaxTaggedPublishedFiles in a single screenshot. Tagging more items than that will just be discarded.
	 * This function has a built in delay before saving the tag which allows you to call it repeatedly for each item.
	 * You can get the handle to tag the screenshot once it has been successfully saved from the ScreenshotReady_t callback or via the WriteScreenshot, AddScreenshotToLibrary, AddVRScreenshotToLibrary calls.
	 *
	 * @param FScreenshotHandle ScreenshotHandle - The handle to the screenshot to tag.
	 * @param FPublishedFileId PublishedFileID - The workshop item ID that is in the screenshot.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	bool TagPublishedFile(const FScreenshotHandle ScreenshotHandle, const FPublishedFileId PublishedFileID) const { return SteamScreenshots()->TagPublishedFile(ScreenshotHandle, PublishedFileID); }

	/**
	 * Tags a Steam user as being visible in the screenshot.
	 * You can tag up to the value declared by k_nScreenshotMaxTaggedUsers in a single screenshot. Tagging more users than that will just be discarded.
	 * This function has a built in delay before saving the tag which allows you to call it repeatedly for each item.
	 * You can get the handle to tag the screenshot once it has been successfully saved from the ScreenshotReady_t callback or via the WriteScreenshot, AddScreenshotToLibrary, AddVRScreenshotToLibrary calls.
	 *
	 * @param FScreenshotHandle ScreenshotHandle - The handle to the screenshot to tag.
	 * @param FSteamID SteamID - The Steam ID of a user that is in the screenshot.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	bool TagUser(const FScreenshotHandle ScreenshotHandle, const FSteamID SteamID) const { return SteamScreenshots()->TagUser(ScreenshotHandle, SteamID); }

	/**
	 * Either causes the Steam Overlay to take a screenshot, or tells your screenshot manager that a screenshot needs to be taken. Depending on the value of IsScreenshotsHooked.
	 * Triggers a ScreenshotRequested_t callback.
	 * Triggers a ScreenshotReady_t callback.
	 * Only causes ScreenshotRequested_t if hooking has been enabled with HookScreenshots. Otherwise ScreenshotReady_t will be called when the screenshot has been saved and added to the library.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Screenshots")
	void TriggerScreenshot() { SteamScreenshots()->TriggerScreenshot(); }

	/**
	 * Writes a screenshot to the user's Steam screenshot library given the raw image data, which must be in RGB format.
	 * Triggers a ScreenshotReady_t callback.
	 * This call is asynchronous, the ScreenshotReady_t callback will be sent when the screenshot has finished writing to disk.
	 *
	 * @param UTexture2D * Image - The image to write.
	 * @return FScreenshotHandle - The handle to this screenshot that is valid for the duration of the game process and can be used to apply tags. Returns INVALID_SCREENSHOT_HANDLE if the file could not be saved.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Screenshots")
	FScreenshotHandle WriteScreenshot(UTexture2D* Image) const;

	/** Delegates */

	/** A screenshot successfully written or otherwise added to the library and can now be tagged. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Screenshots|Delegates", meta = (DisplayName = "OnScreenshotReady"))
	FOnScreenshotReadyDelegate OnScreenshotReadyDelegate;

	/** A screenshot has been requested by the user from the Steam screenshot hotkey. This will only be called if HookScreenshots has been enabled, in which case Steam will not take the screenshot itself. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Screenshots|Delegates", meta = (DisplayName = "OnScreenshotRequested"))
	FOnScreenshotRequestedDelegate OnScreenshotRequestedDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamScreenshots, OnScreenshotReady, ScreenshotReady_t, OnScreenshotReadyCallback);
	STEAM_CALLBACK_MANUAL(USteamScreenshots, OnScreenshotRequested, ScreenshotRequested_t, OnScreenshotRequestedCallback);
};

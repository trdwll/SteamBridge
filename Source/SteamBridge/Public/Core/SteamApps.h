// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamApps.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDlcInstalledDelegate, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnFileDetailsResultDelegate, ESteamResult, Result, int64, FileSize, FString, FileSHA, int32, flags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewUrlLaunchParametersDelegate);

/**
 * Exposes a wide range of information and actions for applications and Downloadable Content (DLC).
 * https://partner.steamgames.com/doc/api/ISteamApps
 */
UCLASS()
class STEAMBRIDGE_API USteamApps final : public UObject
{
	GENERATED_BODY()

public:
	USteamApps();
	~USteamApps();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Apps", CompactNodeTitle = "SteamApps"))
	static USteamApps* GetSteamApps() { return USteamApps::StaticClass()->GetDefaultObject<USteamApps>(); }

	/**
	 * Returns metadata for a DLC by index.
	 *
	 * @param int32 DLC - Index of the DLC to get between 0 and GetDLCCount.
	 * @param int32 & AppID - Returns the App ID of the DLC.
	 * @param bool & bAvailable - Returns whether the DLC is currently available.
	 * @param FString & Name - Returns the name of the DLC.
	 * @return bool - true if the current App ID has DLC's associated with it and iDLC falls between the range of 0 to GetDLCCount, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BGetDLCDataByIndex(int32 DLC, int32& AppID, bool& bAvailable, FString& Name);

	/**
	 * Checks if a specific app is installed.
	 * The app may not actually be owned by the current user, they may have it left over from a free weekend, etc.
	 * This only works for base applications, not Downloadable Content (DLC). Use BIsDlcInstalled for DLC instead.
	 *
	 * @param int32 AppID - The App ID of the application to check.
	 * @return bool - true if the specified App ID is installed; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsAppInstalled(int32 AppID) const { return SteamApps()->BIsAppInstalled(AppID); }

	/**
	 * Checks if the user owns a specific DLC and if the DLC is installed
	 * Note: Should only be used for simple client side checks - not intended for granting in-game items.
	 *
	 * @param int32 AppID - The App ID of the DLC to check.
	 * @return bool - true if the user owns the DLC and it's currently installed, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsDlcInstalled(int32 AppID) const { return SteamApps()->BIsDlcInstalled(AppID); }

	/**
	 * Checks if the license owned by the user provides low violence depots.
	 * Low violence depots are useful for copies sold in countries that have content restrictions.
	 *
	 * @return bool - true if the license owned by the user provides low violence depots; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsLowViolence() const { return SteamApps()->BIsLowViolence(); }

	/**
	 * Checks if the active user is subscribed to the current App ID.
	 * This will always return true if you're using Steam DRM or calling SteamAPI_RestartAppIfNecessary.
	 *
	 * @return bool - true if the active user owns the current AppId, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribed() const { return SteamApps()->BIsSubscribed(); }

	/**
	 * Checks if the active user is subscribed to a specified AppId.
	 * Only use this if you need to check ownership of another game related to yours, a demo for example.
	 *
	 * @param int32 AppID - The App ID to check.
	 * @return bool - true if the active user is subscribed to the specified App ID, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedApp(int32 AppID) const { return SteamApps()->BIsSubscribedApp(AppID); }

	/**
	 * Checks if the active user is accessing the current appID via a temporary Family Shared license owned by another user.
	 * If you need to determine the steamID of the permanent owner of the license, use GetAppOwner.
	 *
	 * @return bool - true if the active user is accessing the current appID via family sharing, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedFromFamilySharing() const { return SteamApps()->BIsSubscribedFromFamilySharing(); }

	/**
	 * Checks if the user is subscribed to the current App ID through a free weekend.
	 * Before using this please contact a Valve technical account manager via the Steamworks Discussion Board to properly package and secure your free weekend.
	 *
	 * @return bool - true if the active user is subscribed to the current App Id via a free weekend otherwise false any other type of license.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedFromFreeWeekend() const { return SteamApps()->BIsSubscribedFromFreeWeekend(); }

	/**
	 * Checks if the user has a VAC ban on their account
	 *
	 * @return bool - true if the user has a VAC ban on their account; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsVACBanned() const { return SteamApps()->BIsVACBanned(); }

	/**
	 * Gets the buildid of this app, may change at any time based on backend updates to the game.
	 *
	 * @return int32 - The current Build Id of this App. Defaults to 0 if you're not running a build downloaded from steam.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetAppBuildId() const { return SteamApps()->GetAppBuildId(); }

	/**
	 * Gets the install folder for a specific AppID.
	 * This works even if the application is not installed, based on where the game would be installed with the default Steam library location.
	 *
	 * @param int32 AppID - The App ID to get the install dir for.
	 * @param FString & Folder - The string buffer that the folder path will be copied into.
	 * @return int32 - Returns the install directory path as a string into the buffer provided in pchFolder and returns the number of bytes that were copied into that buffer.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetAppInstallDir(int32 AppID, FString& Folder);

	/**
	 * Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
	 *
	 * @return FSteamID - The original owner of the current app.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FSteamID GetAppOwner() const { return SteamApps()->GetAppOwner().ConvertToUint64(); }

	/**
	 * Gets a comma separated list of the languages the current app supports.
	 *
	 * @return FString - Returns a comma separated list of languages.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FString GetAvailableGameLanguages() const { return SteamApps()->GetAvailableGameLanguages(); }

	/**
	 * Checks if the user is running from a beta branch, and gets the name of the branch if they are.
	 *
	 * @param FString & Name - The buffer where the beta name will be copied in to.
	 * @return bool - true if the user is on a beta branch; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool GetCurrentBetaName(FString& Name);

	/**
	 * Gets the current language that the user has set.
	 * This falls back to the Steam UI language if the user hasn't explicitly picked a language for the title.
	 *
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FString GetCurrentGameLanguage() const { return SteamApps()->GetCurrentGameLanguage(); }

	/**
	 * Gets the number of DLC pieces for the current app.
	 * This is typically used to loop through each piece of DLC and get the info about each one with BGetDLCDataByIndex.
	 *
	 * @return int32 - The number of DLC pieces for the current app. Note that this value may max out at 64, depending on how much unowned DLC the user has. If your app has a large number of DLC, you should set your -
	 * own internal list of known DLC to check against.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetDLCCount() const { return SteamApps()->GetDLCCount(); }

	/**
	 * Gets the download progress for optional DLC.
	 *
	 * @param int32 AppID - The App ID of the DLC to monitor
	 * @param int64 & BytesDownloaded - Returns the number of bytes downloaded.
	 * @param int64 & BytesTotal - Returns the total size of the download in bytes.
	 * @return bool - true if the specified DLC exists and is currently downloading; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool GetDlcDownloadProgress(int32 AppID, int64& BytesDownloaded, int64& BytesTotal) const { return SteamApps()->GetDlcDownloadProgress(AppID, (uint64*)&BytesDownloaded, (uint64*)&BytesTotal); }

	/**
	 * Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
	 * This is useful for rewarding users based on their initial purchase date.
	 *
	 * @param int32 AppID - The App ID to get the purchase time for.
	 * @return int32 - The earliest purchase time in Unix epoch format (seconds since Jan 1st, 1970).
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetEarliestPurchaseUnixTime(int32 AppID) const { return SteamApps()->GetEarliestPurchaseUnixTime(AppID); }

	/**
	 * NOTE: This method has been rewritten to use FDateTime instead of epoch time. GetEarliestPurchaseUnixTime is the original method.
	 * Gets the time of purchase of the specified app in a readable format.
	 * This is useful for rewarding users based on their initial purchase date.
	 *
	 * @param int32 AppID - The App ID to get the purchase time for.
	 * @return FDateTime - The earliest purchase time in a readable format.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FDateTime GetEarliestPurchaseDateTime(int32 AppID) const { return FDateTime::FromUnixTimestamp(SteamApps()->GetEarliestPurchaseUnixTime(AppID)); }

	/**
	 * Asynchronously retrieves metadata details about a specific file in the depot manifest.
	 *
	 * @param const FString & FileName - The absolute path and name to the file.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a FileDetailsResult_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FSteamAPICall GetFileDetails(const FString& FileName) const { return SteamApps()->GetFileDetails(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Gets a list of all installed depots for a given App ID in mount order.
	 *
	 * @param int32 AppID - The App to list the depots for.
	 * @param TArray<int32> & Depots - A preallocated array that will be filled with the list of depots.
	 * @param int32 MaxDepots - The maximum number of depots to obtain, typically the size of Depots.
	 * @return int32 - The number of depots returned.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetInstalledDepots(int32 AppID, TArray<int32>& Depots, int32 MaxDepots = 1) { return SteamApps()->GetInstalledDepots(AppID, (DepotId_t*)Depots.GetData(), MaxDepots); }

	/**
	 * Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, -
	 * which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the -
	 * Installation > General page on your app.
	 *
	 * @param FString & CommandLine - The string buffer that the command line will be copied into.
	 * @return int32 - Returns the command line as a string into the buffer provided in CommandLine and returns the number of bytes that were copied into that buffer.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetLaunchCommandLine(FString& CommandLine);

	/**
	 * Gets the associated launch parameter if the game is run via steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
	 * Parameter names starting with the character '@' are reserved for internal use and will always return an empty string.
	 * Parameter names starting with an underscore '_' are reserved for steam features -- they can be queried by the game, but it is advised that you not param names beginning with an underscore for your own features.
	 *
	 * @param const FString & Key - The launch key to test for. Ex: param1
	 * @return FString - The value associated with the key provided. Returns an empty string ("") if the specified key does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FString GetLaunchQueryParam(const FString& Key) const { return SteamApps()->GetLaunchQueryParam(TCHAR_TO_UTF8(*Key)); }

	/**
	 * Allows you to install an optional DLC.
	 * Triggers a DlcInstalled_t callback.
	 *
	 * @param int32 AppID - The DLC you want to install.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Apps")
	void InstallDLC(int32 AppID) { SteamApps()->InstallDLC(AppID); }

	/**
	 * Allows you to force verify game content on next launch.
	 * If you detect the game is out-of-date (for example, by having the client detect a version mismatch with a server), you can call use MarkContentCorrupt to force a verify, show a message to the user, and then quit.
	 *
	 * @param bool bMissingFilesOnly - 	Only scan for missing files, don't verify the checksum of each file.
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool MarkContentCorrupt(bool bMissingFilesOnly) const { return SteamApps()->MarkContentCorrupt(bMissingFilesOnly); }

	/**
	 * Allows you to uninstall an optional DLC.
	 *
	 * @param int32 AppID - The DLC you want to uninstall.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Apps")
	void UninstallDLC(int32 AppID) { SteamApps()->UninstallDLC(AppID); }

	/** Delegates */

	/** Triggered after the current user gains ownership of DLC and that DLC is installed. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnDlcInstalled"))
	FOnDlcInstalledDelegate m_OnDlcInstalled;

	/** Called after requesting the details of a specific file. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnFileDetailsResult"))
	FOnFileDetailsResultDelegate m_OnFileDetailsResult;

	/**
	 * Posted after the user executes a steam url with command line or query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running.
	 * The new params can be queried with GetLaunchCommandLine and GetLaunchQueryParam.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnNewUrlLaunchParameters"))
	FOnNewUrlLaunchParametersDelegate m_OnNewUrlLaunchParameters;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamApps, OnDlcInstalled, DlcInstalled_t, OnDlcInstalledCallback);
	STEAM_CALLBACK_MANUAL(USteamApps, OnFileDetailsResult, FileDetailsResult_t, OnFileDetailsResultCallback);
	STEAM_CALLBACK_MANUAL(USteamApps, OnNewUrlLaunchParameters, NewUrlLaunchParameters_t, OnNewUrlLaunchParametersCallback);
};

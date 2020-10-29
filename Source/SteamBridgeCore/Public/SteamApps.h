// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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
class STEAMBRIDGECORE_API USteamApps final : public UObject
{
	GENERATED_BODY()

public:
	USteamApps();
	~USteamApps();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "Steam Apps", CompactNodeTitle = "SteamApps"))
	static USteamApps* GetApps() { return USteamApps::StaticClass()->GetDefaultObject<USteamApps>(); }

	/**
	 * Returns metadata for a DLC by index.
	 *
	 * @param int32 DLC
	 * @param int32 & AppID
	 * @param bool & bAvailable
	 * @param FString & Name
	 * @param int32 NameMaxLength
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BGetDLCDataByIndex(int32 DLC, int32& AppID, bool& bAvailable, FString& Name, int32 NameMaxLength);

	/**
	 * Checks if a specific app is installed.
	 * The app may not actually be owned by the current user, they may have it left over from a free weekend, etc.
	 * This only works for base applications, not Downloadable Content (DLC). Use BIsDlcInstalled for DLC instead.
	 *
	 * @param int32 AppID
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsAppInstalled(int32 AppID) const { return SteamApps()->BIsAppInstalled(AppID); }

	/**
	 * Checks if the user owns a specific DLC and if the DLC is installed
	 *
	 * @param int32 AppID
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsDlcInstalled(int32 AppID) const { return SteamApps()->BIsDlcInstalled(AppID); }

	/**
	 * Checks if the license owned by the user provides low violence depots.
	 * Low violence depots are useful for copies sold in countries that have content restrictions.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsLowViolence() const { return SteamApps()->BIsLowViolence(); }

	/**
	 * Checks if the active user is subscribed to the current App ID.
	 * This will always return true if you're using Steam DRM or calling SteamAPI_RestartAppIfNecessary.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribed() const { return SteamApps()->BIsSubscribed(); }

	/**
	 * Checks if the active user is subscribed to a specified AppId.
	 * Only use this if you need to check ownership of another game related to yours, a demo for example.
	 *
	 * @param int32 AppID
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedApp(int32 AppID) const { return SteamApps()->BIsSubscribedApp(AppID); }

	/**
	 * Checks if the active user is accessing the current appID via a temporary Family Shared license owned by another user.
	 * If you need to determine the steamID of the permanent owner of the license, use GetAppOwner.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedFromFamilySharing() const { return SteamApps()->BIsSubscribedFromFamilySharing(); }

	/**
	 * Checks if the user is subscribed to the current App ID through a free weekend.
	 * Before using this please contact a Valve technical account manager via the Steamworks Discussion Board to properly package and secure your free weekend.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsSubscribedFromFreeWeekend() const { return SteamApps()->BIsSubscribedFromFreeWeekend(); }

	/**
	 * Checks if the user has a VAC ban on their account
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool BIsVACBanned() const { return SteamApps()->BIsVACBanned(); }

	/**
	 * Gets the buildid of this app, may change at any time based on backend updates to the game.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetAppBuildId() const { return SteamApps()->GetAppBuildId(); }

	/**
	 * Gets the install folder for a specific AppID.
	 * This works even if the application is not installed, based on where the game would be installed with the default Steam library location.
	 *
	 * @param int32 AppID
	 * @param FString & Folder
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetAppInstallDir(int32 AppID, FString& Folder);

	/**
	 * Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
	 *
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FSteamID GetAppOwner() const { return SteamApps()->GetAppOwner().ConvertToUint64(); }

	/**
	 * Gets a comma separated list of the languages the current app supports.
	 *
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FString GetAvailableGameLanguages() const { return SteamApps()->GetAvailableGameLanguages(); }

	/**
	 * Checks if the user is running from a beta branch, and gets the name of the branch if they are.
	 *
	 * @param FString & Name
	 * @return bool
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
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetDLCCount() const { return SteamApps()->GetDLCCount(); }

	/**
	 * Gets the download progress for optional DLC.
	 *
	 * @param int32 AppID
	 * @param int64 & BytesDownloaded
	 * @param int64 & BytesTotal
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool GetDlcDownloadProgress(int32 AppID, int64& BytesDownloaded, int64& BytesTotal);

	/**
	 * Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
	 * This is useful for rewarding users based on their initial purchase date.
	 *
	 * @param int32 AppID
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetEarliestPurchaseUnixTime(int32 AppID) const { return SteamApps()->GetEarliestPurchaseUnixTime(AppID); }

	/**
	 * Asynchronously retrieves metadata details about a specific file in the depot manifest.
	 *
	 * @param const FString & FileName
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FSteamAPICall GetFileDetails(const FString& FileName) const { return SteamApps()->GetFileDetails(TCHAR_TO_ANSI(*FileName)); }

	/**
	 * Gets a list of all installed depots for a given App ID in mount order.
	 *
	 * @param int32 AppID
	 * @param TArray<int32> & Depots
	 * @param int32 MaxDepots
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetInstalledDepots(int32 AppID, TArray<int32>& Depots, int32 MaxDepots) { return SteamApps()->GetInstalledDepots(AppID, (DepotId_t*)Depots.GetData(), MaxDepots); }

	/**
	 * Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, -
	 * which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the -
	 * Installation > General page on your app.
	 *
	 * @param FString & CommandLine
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	int32 GetLaunchCommandLine(FString& CommandLine);

	/**
	 * Gets the associated launch parameter if the game is run via steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
	 * Parameter names starting with the character '@' are reserved for internal use and will always return an empty string.
	 * Parameter names starting with an underscore '_' are reserved for steam features -- they can be queried by the game, but it is advised that you not param names beginning with an underscore for your own features.
	 *
	 * @param const FString & Key
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	FString GetLaunchQueryParam(const FString& Key) const { return SteamApps()->GetLaunchQueryParam(TCHAR_TO_UTF8(*Key)); }

	/**
	 * Allows you to install an optional DLC.
	 *
	 * @param int32 DLCAppID
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Apps")
	void InstallDLC(int32 DLCAppID) { SteamApps()->InstallDLC(DLCAppID); }

	/**
	 * Allows you to force verify game content on next launch.
	 * If you detect the game is out-of-date (for example, by having the client detect a version mismatch with a server), you can call use MarkContentCorrupt to force a verify, show a message to the user, and then quit.
	 *
	 * @param bool bMissingFilesOnly
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Apps")
	bool MarkContentCorrupt(bool bMissingFilesOnly) const { return SteamApps()->MarkContentCorrupt(bMissingFilesOnly); }

	/**
	 * Allows you to uninstall an optional DLC.
	 *
	 * @param int32 DLCAppID
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Apps")
	void UninstallDLC(int32 DLCAppID) { SteamApps()->UninstallDLC(DLCAppID); }

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnDlcInstalled"))
	FOnDlcInstalledDelegate m_OnDlcInstalled;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnFileDetailsResult"))
	FOnFileDetailsResultDelegate m_OnFileDetailsResult;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Apps", meta = (DisplayName = "OnNewUrlLaunchParameters"))
	FOnNewUrlLaunchParametersDelegate m_OnNewUrlLaunchParameters;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamApps, OnDlcInstalled, DlcInstalled_t, OnDlcInstalledCallback);
	STEAM_CALLBACK_MANUAL(USteamApps, OnFileDetailsResult, FileDetailsResult_t, OnFileDetailsResultCallback);
	STEAM_CALLBACK_MANUAL(USteamApps, OnNewUrlLaunchParameters, NewUrlLaunchParameters_t, OnNewUrlLaunchParametersCallback);
};

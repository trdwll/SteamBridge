// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamUserStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobalAchievementPercentagesReadyDelegate, int64, GameID, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobalStatsReceivedDelegate, int64, GameID, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNumberOfCurrentPlayersDelegate, bool, bSuccess, int32, PlayersNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUserAchievementIconFetchedDelegate, int64, GameID, FString, AchievementName, bool, bAchieved, int32, IconHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnUserAchievementStoredDelegate, int64, GameID, bool, bGroupAchievement, FString, AchievementName, int32, CurProgress, int32, MaxProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUserStatsReceivedDelegate, int64, GameID, ESteamResult, Result, FSteamID, SteamIDUser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUserStatsStoredDelegate, int64, GameID, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserStatsUnloadedDelegate, FSteamID, SteamIDUser);

/**
 * Provides functions for accessing and submitting stats, achievements, and leaderboards.
 * https://partner.steamgames.com/doc/api/ISteamUserStats
 */
UCLASS()
class STEAMBRIDGE_API USteamUserStats final : public UObject
{
	GENERATED_BODY()

public:
	USteamUserStats();
	~USteamUserStats();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam User Stats", CompactNodeTitle = "SteamUserStats"))
	static USteamUserStats* GetSteamUserStats() { return USteamUserStats::StaticClass()->GetDefaultObject<USteamUserStats>(); }


	/**
	 * Resets the unlock status of an achievement.
	 * This is primarily only ever used for testing.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server and to trigger the Steam overlay notification you must call StoreStats.
	 *
	 * @param const FString & Name - The 'API Name' of the Achievement to reset.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified achievement "API Name" exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool ClearAchievement(const FString& Name) const { return SteamUserStats()->ClearAchievement(TCHAR_TO_UTF8(*Name)); }

	/**
	 * Gets the unlock status of the Achievement.
	 * The equivalent function for other users is GetUserAchievement.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param bool & bAchieved - Returns the unlock status of the achievement.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The 'API Name' of the specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * If the call is successful then the unlock status is returned via the pbAchieved parameter.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievement(const FString& Name, bool& bAchieved) const { return SteamUserStats()->GetAchievement(TCHAR_TO_UTF8(*Name), &bAchieved); }

	/**
	 * Returns the percentage of users who have unlocked the specified achievement.
	 * You must have called RequestGlobalAchievementPercentages and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param float & Percent - Variable to return the percentage of people that have unlocked this achievement from 0 to 100.
	 * @return bool - Returns true upon success; otherwise false if RequestGlobalAchievementPercentages has not been called or if the specified 'API Name' does not exist in the global achievement percentages.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievementAchievedPercent(const FString& Name, float& Percent) const { return SteamUserStats()->GetAchievementAchievedPercent(TCHAR_TO_UTF8(*Name), &Percent); }

	/**
	 * Gets the achievement status, and the time it was unlocked if unlocked.
	 * If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in a friendly format.
	 * The equivalent function for other users is GetUserAchievementAndUnlockTime.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param bool & bAchieved - Returns whether the current user has unlocked the achievement.
	 * @param FDateTime & UnlockTime - Returns the time that the achievement was unlocked; if pbAchieved is true.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The 'API Name' of the specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * If the call is successful then the achieved status and unlock time are provided via the arguments pbAchieved and punUnlockTime.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievementAndUnlockTime(const FString& Name, bool& bAchieved, FDateTime& UnlockTime) const;

	/**
	 * Get general attributes for an achievement. Currently provides: Name, Description, and Hidden status.
	 * This receives the value from a dictionary/map keyvalue store, so you must provide one of the following keys.
	 * "name" to retrive the localized achievement name in UTF8
	 * "desc" to retrive the localized achievement description in UTF8
	 * "hidden" for retrieving if an achievement is hidden. Returns "0" when not hidden, "1" when hidden
	 * This localization is provided based on the games language if it's set, otherwise it checks if a localization is avilable for the users Steam UI Language. If that fails too, then it falls back to english.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param const FString & Key - The 'key' to get a value for.
	 * @return FString - This function returns the value as a string upon success if all of the following conditions are met; otherwise, an empty string: "".
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * The specified pchKey is valid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	FString GetAchievementDisplayAttribute(const FString& Name, const FString& Key) const { return SteamUserStats()->GetAchievementDisplayAttribute(TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Key)); }

	/**
	 * Gets the icon for an achievement.
	 * Triggers a UserAchievementIconFetched_t callback.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @return int32 - The image is returned as a handle to be used with ISteamUtils::GetImageRGBA to get the actual image data.
	 * An invalid handle of 0 will be returned under the following conditions:
	 * RequestCurrentStats has not completed and successfully returned its callback.
	 * The specified achievement does not exist in App Admin on the Steamworks website, or the changes are not published.
	 * Steam is still fetching the image data from the server. This will trigger a UserAchievementIconFetched_t callback which will notify you when the image data is ready and provide you with a new handle.
	 * If the m_nIconHandle in the callback is still 0, then there is no image set for the specified achievement.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetAchievementIcon(const FString& Name) const { return SteamUserStats()->GetAchievementIcon(TCHAR_TO_UTF8(*Name)); }

	/**
	 * Gets the 'API name' for an achievement index between 0 and GetNumAchievements.
	 * This function must be used in cojunction with GetNumAchievements to loop over the list of achievements.
	 * In general games should not need these functions as they should have the list of achievements compiled into them.
	 *
	 * @param int32 AchievementIndex - Index of the achievement.
	 * @return FString - The 'API Name' of the achievement, returns an empty string if iAchievement is not a valid index. RequestCurrentStats must have been called and successfully returned its callback, and the current
	 * App ID must have achievements.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	FString GetAchievementName(int32 AchievementIndex = 0) const { return SteamUserStats()->GetAchievementName(AchievementIndex); }

	/**
	 * Gets the lifetime totals for an aggregated stat.
	 * You must have called RequestGlobalStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param const FString & StatName - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param int64 & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestGlobalStats has completed and successfully returned its callback.
	 * The type matches the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetGlobalStatInt64(const FString& StatName, int64& Data) const { return SteamUserStats()->GetGlobalStat(TCHAR_TO_UTF8(*StatName), &Data); }

	/**
	 * Gets the lifetime totals for an aggregated stat.
	 * You must have called RequestGlobalStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param const FString & StatName - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestGlobalStats has completed and successfully returned its callback.
	 * The type matches the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetGlobalStatFloat(const FString& StatName, float& Data) const { return SteamUserStats()->GetGlobalStat(TCHAR_TO_UTF8(*StatName), (double*)&Data); }

	/**
	 * Gets the daily history for an aggregated stat. pData will be filled with daily values, starting with today. So when called, pData[0] will be today, pData[1] will be yesterday, and pData[2] will be two days ago, etc.
	 * You must have called RequestGlobalStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param const FString & StatName - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param TArray<int64> & Data - Array that the daily history will be returned into.
	 * @param int32 Size - The total size in bytes of the pData array.
	 * @return int32 - The number of elements returned in the pData array.
	 * A value of 0 indicates failure for one of the following reasons:
	 * The specified stat does not exist in App Admin on the Steamworks website, or the changes aren't published.
	 * RequestGlobalStats has not been called or returned its callback, with at least 1 day of history.
	 * The type does not match the type listed in the App Admin panel of the Steamworks website.
	 * There is no history available.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetGlobalStatHistoryInt64(const FString& StatName, TArray<int64>& Data, int32 Size = 10) const { return SteamUserStats()->GetGlobalStatHistory(TCHAR_TO_UTF8(*StatName), Data.GetData(), Size); }

	/**
	 * Gets the daily history for an aggregated stat. pData will be filled with daily values, starting with today. So when called, pData[0] will be today, pData[1] will be yesterday, and pData[2] will be two days ago, etc.
	 * You must have called RequestGlobalStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param const FString & StatName - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param TArray<float> & Data - Array that the daily history will be returned into.
	 * @param int32 Size - The total size in bytes of the pData array.
	 * @return int32 - The number of elements returned in the pData array.
	 * A value of 0 indicates failure for one of the following reasons:
	 * The specified stat does not exist in App Admin on the Steamworks website, or the changes aren't published.
	 * RequestGlobalStats has not been called or returned its callback, with at least 1 day of history.
	 * The type does not match the type listed in the App Admin panel of the Steamworks website.
	 * There is no history available.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetGlobalStatHistoryFloat(const FString& StatName, TArray<float>& Data, int32 Size = 10) const;

	/**
	 * Gets the info on the most achieved achievement for the game.
	 * You must have called RequestGlobalAchievementPercentages and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param FString & Name - String buffer to return the 'API Name' of the achievement into.
	 * @param float & Percent - Variable to return the percentage of people that have unlocked this achievement from 0 to 100.
	 * @param bool & bAchieved - Variable to return whether the current user has unlocked this achievement.
	 * @return int32 - Returns -1 if RequestGlobalAchievementPercentages has not been called or if there are no global achievement percentages for this app Id.
	 * If the call is successful it returns an iterator which should be used with GetNextMostAchievedAchievementInfo.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetMostAchievedAchievementInfo(FString& Name, float& Percent, bool& bAchieved) const;

	/**
	 * Gets the info on the next most achieved achievement for the game.
	 * You must have called RequestGlobalAchievementPercentages and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param int32 IteratorPrevious - Iterator returned from the previous call to this function or from GetMostAchievedAchievementInfo
	 * @param FString & Name - String buffer to return the 'API Name' of the achievement into.
	 * @param float & Percent - Variable to return the percentage of people that have unlocked this achievement from 0 to 100.
	 * @param bool & bAchieved - Variable to return whether the current user has unlocked this achievement.
	 * @return int32 - Returns -1 if RequestGlobalAchievementPercentages has not been called or if there are no global achievement percentages for this app Id.
	 * If the call is successful it returns an iterator which should be used with subsequent calls to this function.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetNextMostAchievedAchievementInfo(int32 IteratorPrevious, FString& Name, float& Percent, bool& bAchieved) const;

	/**
	 * Get the number of achievements defined in the App Admin panel of the Steamworks website.
	 * This is used for iterating through all of the achievements with GetAchievementName.
	 * In general games should not need these functions because they should have a list of existing achievements compiled into them.
	 *
	 * @return int32 - The number of achievements. Returns 0 if RequestCurrentStats has not been called and successfully returned its callback, or the current App ID has no achievements.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetNumAchievements() const { return SteamUserStats()->GetNumAchievements(); }

	/**
	 * Asynchronously retrieves the total number of players currently playing the current game. Both online and in offline mode.
	 *
	 * @return FSteamAPICall - SteamAPICall_t to be used with a NumberOfCurrentPlayers_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall GetNumberOfCurrentPlayers() const { return SteamUserStats()->GetNumberOfCurrentPlayers(); }

	/**
	 * Gets the current value of the a stat for the current user.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this.
	 * To receive stats for other users use GetUserStat.
	 *
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param int32 & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetStatInt32(const FString& Name, int32& Data) const { return SteamUserStats()->GetStat(TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Gets the current value of the a stat for the current user.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this.
	 * To receive stats for other users use GetUserStat.
	 *
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetStatFloat(const FString& Name, float& Data) const { return SteamUserStats()->GetStat(TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Gets the unlock status of the Achievement.
	 * The equivalent function for the local user is GetAchievement, the equivalent function for game servers is ISteamGameServerStats::GetUserAchievement.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the achievement for.
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param bool & bAchieved - Returns the unlock status of the achievement.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The 'API Name' of the specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * If the call is successful then the unlock status is returned via the pbAchieved parameter.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetUserAchievement(FSteamID SteamIDUser, const FString& Name, bool& bAchieved) const { return SteamUserStats()->GetUserAchievement(SteamIDUser, TCHAR_TO_UTF8(*Name), &bAchieved); }

	/**
	 * Gets the achievement status, and the time it was unlocked if unlocked.
	 * If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in a friendly format.
	 * The equivalent function for the local user is GetAchievementAndUnlockTime.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the achievement for.
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param bool & bAchieved - Returns whether the current user has unlocked the achievement.
	 * @param FDateTime & UnlockTime - Returns the time that the achievement was unlocked; if pbAchieved is true.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The 'API Name' of the specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * If the call is successful then the achieved status and unlock time are provided via the arguments pbAchieved and punUnlockTime.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetUserAchievementAndUnlockTime(FSteamID SteamIDUser, const FString& Name, bool& bAchieved, FDateTime& UnlockTime) const;

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 * The equivalent function for the local user is GetStat, the equivalent function for game servers is ISteamGameServerStats::GetUserStat.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the stat for.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param int32 & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The type does not match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetUserStatInt32(FSteamID SteamIDUser, const FString& Name, int32& Data) const { return SteamUserStats()->GetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 * The equivalent function for the local user is GetStat, the equivalent function for game servers is ISteamGameServerStats::GetUserStat.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the stat for.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The type does not match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetUserStatFloat(FSteamID SteamIDUser, const FString& Name, float& Data) const { return SteamUserStats()->GetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Shows the user a pop-up notification with the current progress of an achievement.
	 * Calling this function will NOT set the progress or unlock the achievement, the game must do that manually by calling SetStat!
	 * Triggers a UserStatsStored_t callback.
	 * Triggers a UserAchievementStored_t callback.
	 *
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param int32 CurProgress - The current progress.
	 * @param int32 MaxProgress - The progress required to unlock the achievement.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * The specified achievement is not already unlocked.
 	 * nCurProgress is less than nMaxProgress.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool IndicateAchievementProgress(const FString& Name, int32 CurProgress, int32 MaxProgress) const { return SteamUserStats()->IndicateAchievementProgress(TCHAR_TO_UTF8(*Name), CurProgress, MaxProgress); }

	/**
	 * Asynchronously request the user's current stats and achievements from the server.
	 * You must always call this first to get the initial status of stats and achievements.
	 * Only after the resulting callback comes back can you start calling the rest of the stats and achievement functions for the current user.
	 * The equivalent function for other users is RequestUserStats.
	 * Triggers a UserStatsReceived_t callback.
	 *
	 * @return bool - Only returns false if there is no user logged in; otherwise, true.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool RequestCurrentStats() const { return SteamUserStats()->RequestCurrentStats(); }

	/**
	 * Asynchronously fetch the data for the percentage of players who have received each achievement for the current game globally.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 *
	 * @return FSteamAPICall - SteamAPICall_t to be used with a GlobalAchievementPercentagesReady_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall RequestGlobalAchievementPercentages() const { return SteamUserStats()->RequestGlobalAchievementPercentages(); }

	/**
	 * Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param int32 HistoryDays - How many days of day-by-day history to retrieve in addition to the overall totals. The limit is 60.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a GlobalStatsReceived_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall RequestGlobalStats(int32 HistoryDays) const { return SteamUserStats()->RequestGlobalStats(HistoryDays); }

	/**
	 * Asynchronously downloads stats and achievements for the specified user from the server.
	 * These stats are not automatically updated; you'll need to call this function again to refresh any data that may have change.
	 * To keep from using too much memory, an least recently used cache (LRU) is maintained and other user's stats will occasionally be unloaded. When this happens a UserStatsUnloaded_t callback is sent.
	 * After receiving this callback the user's stats will be unavailable until this function is called again.
	 * The equivalent function for the local user is RequestCurrentStats, the equivalent function for game servers is ISteamGameServerStats::RequestUserStats.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to request stats for.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a UserStatsReceived_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall RequestUserStats(FSteamID SteamIDUser) const { return SteamUserStats()->RequestUserStats(SteamIDUser); }

	/**
	 * Resets the current users stats and, optionally achievements.
	 * This automatically calls StoreStats to persist the changes to the server. This should typically only be used for testing purposes during development. Ensure that you sync up your stats with the new default values
	 * provided by Steam after calling this by calling RequestCurrentStats.
	 *
	 * @param bool bAchievementsToo - Also reset the user's achievements?
	 * @return bool - true indicating success if RequestCurrentStats has been called and successfully returned its callback; otherwise false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool ResetAllStats(bool bAchievementsToo) const { return SteamUserStats()->ResetAllStats(bAchievementsToo); }

	/**
	 * Unlocks an achievement.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 * You can unlock an achievement multiple times so you don't need to worry about only setting achievements that aren't already set. This call only modifies Steam's in-memory state so it is quite cheap.
	 * To send the unlock status to the server and to trigger the Steam overlay notification you must call StoreStats.
	 *
	 * @param const FString & Name - The 'API Name' of the Achievement to unlock.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified achievement "API Name" exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool SetAchievement(const FString& Name) const { return SteamUserStats()->SetAchievement(TCHAR_TO_UTF8(*Name)); }

	/**
	 * Sets / updates the value of a given stat for the current user.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown.
	 * To submit the stats to the server you must call StoreStats.
	 * If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.
	 *
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param int32 Data - The new value of the stat. This must be an absolute value, it will not increment or decrement for you.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool SetStatInt32(const FString& Name, int32 Data) const { return SteamUserStats()->SetStat(TCHAR_TO_UTF8(*Name), Data); }

	/**
	 * Sets / updates the value of a given stat for the current user.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown.
	 * To submit the stats to the server you must call StoreStats.
	 * If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.
	 *
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float Data - The new value of the stat. This must be an absolute value, it will not increment or decrement for you.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool SetStatFloat(const FString& Name, float Data) const { return SteamUserStats()->SetStat(TCHAR_TO_UTF8(*Name), Data); }

	/**
	 * Send the changed stats and achievements data to the server for permanent storage.
	 * If this fails then nothing is sent to the server. It's advisable to keep trying until the call is successful.
	 * This call can be rate limited. Call frequency should be on the order of minutes, rather than seconds. You should only be calling this during major state changes such as the end of a round, the map changing,
	 * or the user leaving a server. This call is required to display the achievement unlock notification dialog though, so if you have called SetAchievement then it's advisable to call this soon after that.
	 * If you have stats or achievements that you have saved locally but haven't uploaded with this function when your application process ends then this function will automatically be called.
	 * You can find additional debug information written to the %steam_install%\logs\stats_log.txt file.
	 * Triggers a UserStatsStored_t callback.
	 * Triggers a UserAchievementStored_t callback.
	 *
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The current game has stats associated with it in the Steamworks Partner backend, and those stats are published.
	 * If the call is successful you will receive a UserStatsStored_t callback.
	 * If m_eResult has a result of k_EResultInvalidParam, then one or more stats uploaded has been rejected, either because they broke constraints or were out of date. In this case the server sends back updated values
	 * and the stats should be updated locally to keep in sync.
	 * If one or more achievements has been unlocked then this will also trigger a UserAchievementStored_t callback.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool StoreStats() const { return SteamUserStats()->StoreStats(); }

	/**
	 * Updates an AVGRATE stat with new values.
	 * You must have called RequestCurrentStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown.
	 * To submit the stats to the server you must call StoreStats.
	 * If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.
	 *
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float CountThisSession - The value accumulation since the last call to this function.
	 * @param float SessionLength - The amount of time in seconds since the last call to this function.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestCurrentStats has completed and successfully returned its callback.
	 * The type must be AVGRATE in the Steamworks Partner backend.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool UpdateAvgRateStat(const FString& Name, float CountThisSession, float SessionLength) const { return SteamUserStats()->UpdateAvgRateStat(TCHAR_TO_UTF8(*Name), CountThisSession, (double)SessionLength); }

	/** Delegates */

	/** Called when the global achievement percentages have been received from the server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnGlobalAchievementPercentagesReady"))
	FOnGlobalAchievementPercentagesReadyDelegate m_OnGlobalAchievementPercentagesReady;

	/** Called when the global stats have been received from the server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnGlobalStatsReceived"))
	FOnGlobalStatsReceivedDelegate m_OnGlobalStatsReceived;

	/** Gets the current number of players for the current AppId. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnNumberOfCurrentPlayers"))
	FOnNumberOfCurrentPlayersDelegate m_OnNumberOfCurrentPlayers;

	/** Result of an achievement icon that has been fetched */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnUserAchievementIconFetched"))
	FOnUserAchievementIconFetchedDelegate m_OnUserAchievementIconFetched;

	/** Result of a request to store the achievements on the server, or an "indicate progress" call. If both m_nCurProgress and m_nMaxProgress are zero, that means the achievement has been fully unlocked. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnUserAchievementStored"))
	FOnUserAchievementStoredDelegate m_OnUserAchievementStored;

	/** Called when the latest stats and achievements for a specific user (including the local user) have been received from the server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnUserStatsReceived"))
	FOnUserStatsReceivedDelegate m_OnUserStatsReceived;

	/** Result of a request to store the user stats.*/
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnUserStatsStored"))
	FOnUserStatsStoredDelegate m_OnUserStatsStored;

	/**
	 * Callback indicating that a user's stats have been unloaded.
	 * Call RequestUserStats again before accessing stats for this user.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UserStats", meta = (DisplayName = "OnUserStatsUnloaded"))
	FOnUserStatsUnloadedDelegate m_OnUserStatsUnloaded;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnGlobalAchievementPercentagesReady, GlobalAchievementPercentagesReady_t, OnGlobalAchievementPercentagesReadyCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnGlobalStatsReceived, GlobalStatsReceived_t, OnGlobalStatsReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnNumberOfCurrentPlayers, NumberOfCurrentPlayers_t, OnNumberOfCurrentPlayersCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnUserAchievementIconFetched, UserAchievementIconFetched_t, OnUserAchievementIconFetchedCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnUserAchievementStored, UserAchievementStored_t, OnUserAchievementStoredCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnUserStatsReceived, UserStatsReceived_t, OnUserStatsReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnUserStatsStored, UserStatsStored_t, OnUserStatsStoredCallback);
	STEAM_CALLBACK_MANUAL(USteamUserStats, OnUserStatsUnloaded, UserStatsUnloaded_t, OnUserStatsUnloadedCallback);
};

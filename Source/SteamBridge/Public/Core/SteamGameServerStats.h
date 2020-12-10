// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamGameServerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGSStatsReceivedDelegate, ESteamResult, Result, FSteamID, SteamIDUser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGSStatsStoredDelegate, ESteamResult, Result, FSteamID, SteamIDUser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSStatsUnloadedDelegate, FSteamID, SteamIDUser);

/**
 * Functions to allow game servers to set stats and achievements on players.
 * https://partner.steamgames.com/doc/api/ISteamGameServerStatsStats
 */
UCLASS()
class STEAMBRIDGE_API USteamGameServerStats final : public UObject
{
	GENERATED_BODY()

public:
	USteamGameServerStats();
	~USteamGameServerStats();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Game Server Stats", CompactNodeTitle = "SteamGameServerStats"))
	static USteamGameServerStats* GetSteamGameServerStats() { return USteamGameServerStats::StaticClass()->GetDefaultObject<USteamGameServerStats>(); }

	/**
	 * Resets the unlock status of an achievement for the specified user.
	 * This is primarily only ever used for testing.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: This will work only on achievements that game servers are allowed to set. If the "Set By" field for this achievement is "Official GS" then only game servers that have been declared as officially -
	 * controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser - 	The Steam ID of the user to clear the achievement for.
	 * @param const FString & Name - The 'API Name' of the Achievement to reset.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 *	The specified achievement "API Name" exists in App Admin on the Steamworks website, and the changes are published.
	 *	RequestUserStats has completed and successfully returned its callback for the specified user.
	 *	The stat must be allowed to be set by game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool ClearUserAchievement(FSteamID SteamIDUser, const FString& Name) const { return SteamGameServerStats()->ClearUserAchievement(SteamIDUser, TCHAR_TO_UTF8(*Name)); }

	/**
	 * Gets the unlock status of the Achievement.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the achievement for.
	 * @param const FString & Name - The 'API Name' of the achievement.
	 * @param bool & bAchieved - Returns the unlock status of the achievement.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * RequestUserStats has completed and successfully returned its callback.
	 * 	The 'API Name' of the specified achievement exists in App Admin on the Steamworks website, and the changes are published.
	 * If the call is successful then the unlock status is returned via the bAchieved parameter.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserAchievement(FSteamID SteamIDUser, const FString& Name, bool& bAchieved) { return SteamGameServerStats()->GetUserAchievement(SteamIDUser, TCHAR_TO_UTF8(*Name), &bAchieved); }

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the stat for.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserStatInt(FSteamID SteamIDUser, const FString& Name, int32& Data) { return SteamGameServerStats()->GetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to get the stat for.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float & Data - The variable to return the stat value into.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserStatFloat(FSteamID SteamIDUser, const FString& Name, float& Data) { return SteamGameServerStats()->GetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), &Data); }

	// #TODO RequestUserStats

	/**
	 * Unlocks an achievement for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to unlock the achievement for.
	 * @param const FString & Name - The 'API Name' of the Achievement to unlock.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified achievement "API Name" exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback for the specified user.
	 * The stat must be allowed to be set by game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserAchievement(FSteamID SteamIDUser, const FString& Name) const { return SteamGameServerStats()->SetUserAchievement(SteamIDUser, TCHAR_TO_UTF8(*Name)); }

	/**
	 * Sets / updates the value of a given stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to set the stat on.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float Data - The new value of the stat. This must be an absolute value, it will not increment or decrement for you.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback for the specified user.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 * The stat must be allowed to be set by game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserStatInt(FSteamID SteamIDUser, const FString& Name, int32 Data) { return SteamGameServerStats()->SetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), Data); }

	/**
	 * Sets / updates the value of a given stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to set the stat on.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float Data - The new value of the stat. This must be an absolute value, it will not increment or decrement for you.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback for the specified user.
	 * The type passed to this function must match the type listed in the App Admin panel of the Steamworks website.
	 * The stat must be allowed to be set by game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserStatFloat(FSteamID SteamIDUser, const FString& Name, float Data) { return SteamGameServerStats()->SetUserStat(SteamIDUser, TCHAR_TO_UTF8(*Name), Data); }

	// #TODO StoreUserStats

	/**
	 * Updates an AVGRATE stat with new values for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser - The Steam ID of the user to update the AVGRATE stat for.
	 * @param const FString & Name - The 'API Name' of the stat. Must not be longer than k_cchStatNameMax.
	 * @param float CountThisSession - The value accumulation since the last call to this function.
	 * @param float SessionLength - The amount of time in seconds since the last call to this function.
	 * @return bool - This function returns true upon success if all of the following conditions are met; otherwise, false.
	 * The specified stat exists in App Admin on the Steamworks website, and the changes are published.
	 * RequestUserStats has completed and successfully returned its callback for the specified user.
	 * The type must be AVGRATE in the Steamworks Partner backend.
	 * The stat must be allowed to be set by game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool UpdateUserAvgRateStat(FSteamID SteamIDUser, const FString& Name, float CountThisSession, float SessionLength) const;

	/** Delegates */

	/** Result when getting the latests stats and achievements for a user from the server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsReceived"))
	FOnGSStatsReceivedDelegate m_OnGSStatsReceived;

	/** Result of a request to store the user stats. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsStored"))
	FOnGSStatsStoredDelegate m_OnGSStatsStored;

	/**
	 * Callback indicating that a user's stats have been unloaded.
	 * Call RequestUserStats again to access stats for this user.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsUnloaded"))
	FOnGSStatsUnloadedDelegate m_OnGSStatsUnloaded;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsReceived, GSStatsReceived_t, OnGSStatsReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsStored, GSStatsStored_t, OnGSStatsStoredCallback);
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsUnloaded, GSStatsUnloaded_t, OnGSStatsUnloadedCallback);
};

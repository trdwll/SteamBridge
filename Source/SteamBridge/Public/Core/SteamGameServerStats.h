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

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "Steam Game Server Stats", CompactNodeTitle = "SteamGameServerStats"))
	static USteamGameServerStats* GetSteamGameServerStats() { return USteamGameServerStats::StaticClass()->GetDefaultObject<USteamGameServerStats>(); }

	/**
	 * Resets the unlock status of an achievement for the specified user.
	 * This is primarily only ever used for testing.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool ClearUserAchievement(FSteamID SteamIDUser, const FString& Name) const { return SteamGameServerStats()->ClearUserAchievement(SteamIDUser.Value, TCHAR_TO_UTF8(*Name)); }

	/**
	 * Gets the unlock status of the Achievement.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param bool & bAchieved
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserAchievement(FSteamID SteamIDUser, const FString& Name, bool& bAchieved) { return SteamGameServerStats()->GetUserAchievement(SteamIDUser.Value, TCHAR_TO_UTF8(*Name), &bAchieved); }

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param float & Data
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserStatInt(FSteamID SteamIDUser, const FString& Name, int32& Data) { return SteamGameServerStats()->GetUserStat(SteamIDUser.Value, TCHAR_TO_UTF8(*Name), &Data); }

	/**
	 * Gets the current value of the a stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param float & Data
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool GetUserStatFloat(FSteamID SteamIDUser, const FString& Name, float& Data) { return SteamGameServerStats()->GetUserStat(SteamIDUser.Value, TCHAR_TO_UTF8(*Name), &Data); }

	// TODO: RequestUserStats

	/**
	 * Unlocks an achievement for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserAchievement(FSteamID SteamIDUser, const FString& Name) const { return SteamGameServerStats()->SetUserAchievement(SteamIDUser.Value, TCHAR_TO_UTF8(*Name)); }

	/**
	 * Sets / updates the value of a given stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param float Data
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserStatInt(FSteamID SteamIDUser, const FString& Name, int32 Data) { return SteamGameServerStats()->SetUserStat(SteamIDUser.Value, TCHAR_TO_UTF8(*Name), Data); }

	/**
	 * Sets / updates the value of a given stat for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param float Data
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool SetUserStatFloat(FSteamID SteamIDUser, const FString& Name, float Data) { return SteamGameServerStats()->SetUserStat(SteamIDUser.Value, TCHAR_TO_UTF8(*Name), Data); }

	// TODO: StoreUserStats

	/**
	 * Updates an AVGRATE stat with new values for the specified user.
	 * You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this!
	 * This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call StoreUserStats.
	 * NOTE: These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as -
	 * officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.
	 *
	 * @param FSteamID SteamIDUser
	 * @param const FString & Name
	 * @param float CountThisSession
	 * @param float SessionLength
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServerStats")
	bool UpdateUserAvgRateStat(FSteamID SteamIDUser, const FString& Name, float CountThisSession, float SessionLength) const;

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsReceived"))
	FOnGSStatsReceivedDelegate m_OnGSStatsReceived;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsStored"))
	FOnGSStatsStoredDelegate m_OnGSStatsStored;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServerStats", meta = (DisplayName = "OnGSStatsUnloaded"))
	FOnGSStatsUnloadedDelegate m_OnGSStatsUnloaded;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsReceived, GSStatsReceived_t, OnGSStatsReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsStored, GSStatsStored_t, OnGSStatsStoredCallback);
	STEAM_CALLBACK_MANUAL(USteamGameServerStats, OnGSStatsUnloaded, GSStatsUnloaded_t, OnGSStatsUnloadedCallback);
};

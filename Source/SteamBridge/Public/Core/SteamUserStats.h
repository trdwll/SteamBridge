// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamUserStats.generated.h"


/**
 * Provides functions to interface with the Steam video and broadcasting platforms.
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

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall AttachLeaderboardUGC(FSteamLeaderboard SteamLeaderboard, FUGCHandle UGC) const { return SteamUserStats()->AttachLeaderboardUGC(SteamLeaderboard, UGC); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	bool ClearAchievement(const FString& Name) const { return SteamUserStats()->ClearAchievement(TCHAR_TO_UTF8(*Name)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall DownloadLeaderboardEntries(FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardDataRequest LeaderboardDataRequest, int32 RangeStart, int32 RangeEnd) const;

	// #TODO: FSteamAPICall DownloadLeaderboardEntriesForUsers(FSteamLeaderboard SteamLeaderboard, TArray<FSteamID>& Users) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall FindLeaderboard(const FString& LeaderboardName) const { return SteamUserStats()->FindLeaderboard(TCHAR_TO_UTF8(*LeaderboardName)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UserStats")
	FSteamAPICall FindOrCreateLeaderboard(const FString& LeaderboardName, ESteamLeaderboardSortMethod LeaderboardSortMethod, ESteamLeaderboardDisplayType LeaderboardDisplayType) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievement(const FString& Name, bool& bAchieved) const { return SteamUserStats()->GetAchievement(TCHAR_TO_UTF8(*Name), &bAchieved); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievementAchievedPercent(const FString& Name, float& Percent) const { return SteamUserStats()->GetAchievementAchievedPercent(TCHAR_TO_UTF8(*Name), &Percent); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetAchievementAndUnlockTime(const FString& Name, bool& bAchieved, int32& UnlockTime) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	FString GetAchievementDisplayAttribute(const FString& Name, const FString& Key) const { return SteamUserStats()->GetAchievementDisplayAttribute(TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Key)); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetAchievementIcon(const FString& Name) const { return SteamUserStats()->GetAchievementIcon(TCHAR_TO_UTF8(*Name)); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	FString GetAchievementName(int32 AchievementIndex = 0) const { return SteamUserStats()->GetAchievementName(AchievementIndex); }


	// #TODO: bool GetDownloadedLeaderboardEntry(FSteamLeaderboardEntries SteamLeaderboardEntries, int32 index, FSteamLeaderboardEntry& LeaderboardEntry, int32& Details, int32 DetailsMax) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetGlobalStatInt64(const FString& StatName, int64& Data) const { return SteamUserStats()->GetGlobalStat(TCHAR_TO_UTF8(*StatName), &Data); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	bool GetGlobalStatFloat(const FString& StatName, float& Data) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetGlobalStatHistoryInt64(const FString& StatName, TArray<int64>& Data, int32 Size = 10) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UserStats")
	int32 GetGlobalStatHistoryFloat(const FString& StatName, TArray<float>& Data, int32 Size = 10) const;




























protected:
private:
};

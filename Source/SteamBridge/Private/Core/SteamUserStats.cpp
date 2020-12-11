// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUserStats.h"

#include "SteamBridgeUtils.h"

USteamUserStats::USteamUserStats()
{
}

USteamUserStats::~USteamUserStats()
{
}

FSteamAPICall USteamUserStats::DownloadLeaderboardEntries(FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardDataRequest LeaderboardDataRequest, int32 RangeStart, int32 RangeEnd) const
{
	return SteamUserStats()->DownloadLeaderboardEntries(SteamLeaderboard, (ELeaderboardDataRequest)LeaderboardDataRequest, RangeStart, RangeEnd);
}

FSteamAPICall USteamUserStats::FindOrCreateLeaderboard(const FString& LeaderboardName, ESteamLeaderboardSortMethod LeaderboardSortMethod, ESteamLeaderboardDisplayType LeaderboardDisplayType) const
{
	return SteamUserStats()->FindOrCreateLeaderboard(TCHAR_TO_UTF8(*LeaderboardName), (ELeaderboardSortMethod)LeaderboardSortMethod, (ELeaderboardDisplayType)LeaderboardDisplayType);
}

bool USteamUserStats::GetAchievementAndUnlockTime(const FString& Name, bool& bAchieved, int32& UnlockTime) const
{
	uint32 TmpTime = 0;
	bool bResult = SteamUserStats()->GetAchievementAndUnlockTime(TCHAR_TO_UTF8(*Name), &bAchieved, &TmpTime);
	UnlockTime = TmpTime;
	return bResult;
}

bool USteamUserStats::GetGlobalStatFloat(const FString& StatName, float& Data) const
{
	double TmpData = 0;
	bool bResult = SteamUserStats()->GetGlobalStat(TCHAR_TO_UTF8(*StatName), &TmpData);
	Data = TmpData;
	return bResult;
}

int32 USteamUserStats::GetGlobalStatHistoryInt64(const FString& StatName, TArray<int64>& Data, int32 Size) const
{
	TArray<int64> TmpData;
	int32 result = SteamUserStats()->GetGlobalStatHistory(TCHAR_TO_UTF8(*StatName), TmpData.GetData(), Size);
	Data = TmpData;
	return result;
}

int32 USteamUserStats::GetGlobalStatHistoryFloat(const FString& StatName, TArray<float>& Data, int32 Size) const
{
	TArray<double> TmpData;
	int32 result = SteamUserStats()->GetGlobalStatHistory(TCHAR_TO_UTF8(*StatName), TmpData.GetData(), Size);
	for (int32 i = 0; i < TmpData.Num(); i++)
	{
		Data.Add((float)TmpData[i]);
	}
	return result;
}

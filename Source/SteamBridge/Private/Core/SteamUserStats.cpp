// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUserStats.h"

#include "SteamBridgeUtils.h"

USteamUserStats::USteamUserStats()
{
	OnGlobalAchievementPercentagesReadyCallback.Register(this, &USteamUserStats::OnGlobalAchievementPercentagesReady);
	OnGlobalStatsReceivedCallback.Register(this, &USteamUserStats::OnGlobalStatsReceived);
	OnNumberOfCurrentPlayersCallback.Register(this, &USteamUserStats::OnNumberOfCurrentPlayers);
	OnUserAchievementIconFetchedCallback.Register(this, &USteamUserStats::OnUserAchievementIconFetched);
	OnUserAchievementStoredCallback.Register(this, &USteamUserStats::OnUserAchievementStored);
	OnUserStatsReceivedCallback.Register(this, &USteamUserStats::OnUserStatsReceived);
	OnUserStatsStoredCallback.Register(this, &USteamUserStats::OnUserStatsStored);
	OnUserStatsUnloadedCallback.Register(this, &USteamUserStats::OnUserStatsUnloaded);
}

USteamUserStats::~USteamUserStats()
{
	OnGlobalAchievementPercentagesReadyCallback.Unregister();
	OnGlobalStatsReceivedCallback.Unregister();
	OnNumberOfCurrentPlayersCallback.Unregister();
	OnUserAchievementIconFetchedCallback.Unregister();
	OnUserAchievementStoredCallback.Unregister();
	OnUserStatsReceivedCallback.Unregister();
	OnUserStatsStoredCallback.Unregister();
	OnUserStatsUnloadedCallback.Unregister();
}


bool USteamUserStats::GetAchievementAndUnlockTime(const FString& Name, bool& bAchieved, FDateTime& UnlockTime) const
{
	uint32 TmpTime;
	bool bResult = SteamUserStats()->GetAchievementAndUnlockTime(TCHAR_TO_UTF8(*Name), &bAchieved, &TmpTime);
	UnlockTime = FDateTime::FromUnixTimestamp(TmpTime);
	return bResult;
}

int32 USteamUserStats::GetGlobalStatHistoryFloat(const FString& StatName, TArray<float>& Data, int32 Size) const
{
	TArray<double> TmpData;
	int32 result = SteamUserStats()->GetGlobalStatHistory(TCHAR_TO_UTF8(*StatName), TmpData.GetData(), Size);
	for (const auto& LData : TmpData)
	{
		Data.Add((float)LData);
	}
	return result;
}

int32 USteamUserStats::GetMostAchievedAchievementInfo(FString& Name, float& Percent, bool& bAchieved) const
{
	TArray<char> TmpName;
	int32 result = SteamUserStats()->GetMostAchievedAchievementInfo(TmpName.GetData(), 1024, &Percent, &bAchieved);
	Name = UTF8_TO_TCHAR(TmpName.GetData());
	return  result;
}

int32 USteamUserStats::GetNextMostAchievedAchievementInfo(int32 IteratorPrevious, FString& Name, float& Percent, bool& bAchieved) const
{
	TArray<char> TmpName;
	int32 result = SteamUserStats()->GetNextMostAchievedAchievementInfo(IteratorPrevious, TmpName.GetData(), 1024, &Percent, &bAchieved);
	Name = UTF8_TO_TCHAR(TmpName.GetData());
	return  result;
}

bool USteamUserStats::GetUserAchievementAndUnlockTime(FSteamID SteamIDUser, const FString& Name, bool& bAchieved, FDateTime& UnlockTime) const
{
	uint32 TmpTime;
	bool bResult = SteamUserStats()->GetUserAchievementAndUnlockTime(SteamIDUser, TCHAR_TO_UTF8(*Name), &bAchieved, &TmpTime);
	UnlockTime = FDateTime::FromUnixTimestamp(TmpTime);
	return bResult;
}

void USteamUserStats::OnGlobalAchievementPercentagesReady(GlobalAchievementPercentagesReady_t* pParam)
{
	m_OnGlobalAchievementPercentagesReady.Broadcast(pParam->m_nGameID, (ESteamResult)pParam->m_eResult);
}

void USteamUserStats::OnGlobalStatsReceived(GlobalStatsReceived_t* pParam)
{
	m_OnGlobalStatsReceived.Broadcast(pParam->m_nGameID, (ESteamResult)pParam->m_eResult);
}

void USteamUserStats::OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t* pParam)
{
	m_OnNumberOfCurrentPlayers.Broadcast(pParam->m_bSuccess == 1, pParam->m_cPlayers);
}

void USteamUserStats::OnUserAchievementIconFetched(UserAchievementIconFetched_t* pParam)
{
	m_OnUserAchievementIconFetched.Broadcast(pParam->m_nGameID.ToUint64(), UTF8_TO_TCHAR(pParam->m_rgchAchievementName), pParam->m_bAchieved, pParam->m_nIconHandle);
}

void USteamUserStats::OnUserAchievementStored(UserAchievementStored_t* pParam)
{
	m_OnUserAchievementStored.Broadcast(pParam->m_nGameID, pParam->m_bGroupAchievement, UTF8_TO_TCHAR(pParam->m_rgchAchievementName), pParam->m_nCurProgress, pParam->m_nMaxProgress);
}

void USteamUserStats::OnUserStatsReceived(UserStatsReceived_t* pParam)
{
	m_OnUserStatsReceived.Broadcast(pParam->m_nGameID, (ESteamResult)pParam->m_eResult, pParam->m_steamIDUser.ConvertToUint64());
}

void USteamUserStats::OnUserStatsStored(UserStatsStored_t* pParam)
{
	m_OnUserStatsStored.Broadcast(pParam->m_nGameID, (ESteamResult)pParam->m_eResult);
}

void USteamUserStats::OnUserStatsUnloaded(UserStatsUnloaded_t* pParam)
{
	m_OnUserStatsUnloaded.Broadcast(pParam->m_steamIDUser.ConvertToUint64());
}

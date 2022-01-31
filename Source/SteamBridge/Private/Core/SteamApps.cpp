// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamApps.h"

#include "SteamBridgeUtils.h"

USteamApps::USteamApps()
{
	OnDlcInstalledCallback.Register(this, &USteamApps::OnDlcInstalled);
	OnFileDetailsResultCallback.Register(this, &USteamApps::OnFileDetailsResult);
	OnNewUrlLaunchParametersCallback.Register(this, &USteamApps::OnNewUrlLaunchParameters);
}

USteamApps::~USteamApps()
{
	OnDlcInstalledCallback.Unregister();
	OnNewUrlLaunchParametersCallback.Unregister();
}

bool USteamApps::BGetDLCDataByIndex(const int32 DLC, int32& AppID, bool& bAvailable, FString& Name)
{
	if (DLC > GetDLCCount())
	{
		return false;
	}

	TArray<char> TempStr;
	TempStr.Reserve(SteamDefs::Buffer1024);
	bool bResult = SteamApps()->BGetDLCDataByIndex(DLC, (uint32*)&AppID, &bAvailable, TempStr.GetData(), SteamDefs::Buffer1024);
	Name = UTF8_TO_TCHAR(TempStr.GetData());
	return bResult;
}

int32 USteamApps::GetAppInstallDir(const int32 AppID, FString& Folder)
{
	TArray<char> TempPath;
	TempPath.SetNum(SteamDefs::Buffer8192);
	uint32 Length = SteamApps()->GetAppInstallDir(AppID, TempPath.GetData(), SteamDefs::Buffer8192);
	if (Length < 0)
	{
		return -1;
	}
	TempPath.SetNum(Length);
	Folder = UTF8_TO_TCHAR(TempPath.GetData());
	return Length;
}

bool USteamApps::GetCurrentBetaName(FString& Name)
{
	TArray<char> TempStr;
	TempStr.Reserve(SteamDefs::Buffer4096);
	bool bResult = SteamApps()->GetCurrentBetaName(TempStr.GetData(), SteamDefs::Buffer4096);
	Name = UTF8_TO_TCHAR(TempStr.GetData());
	return bResult;
}

int32 USteamApps::GetLaunchCommandLine(FString& CommandLine)
{
	TArray<char> TempStr;
	TempStr.Reserve(SteamDefs::Buffer2048);
	int32 res = SteamApps()->GetLaunchCommandLine(TempStr.GetData(), SteamDefs::Buffer2048);
	CommandLine = UTF8_TO_TCHAR(TempStr.GetData());
	return res;
}

void USteamApps::OnDlcInstalled(DlcInstalled_t* pParam)
{
	OnDlcInstalledDelegate.Broadcast((int32)pParam->m_nAppID);
}

void USteamApps::OnFileDetailsResult(FileDetailsResult_t* pParam)
{
	OnFileDetailsResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, (int64)pParam->m_ulFileSize, UTF8_TO_TCHAR(pParam->m_FileSHA), (int32)pParam->m_unFlags);
}

void USteamApps::OnNewUrlLaunchParameters(NewUrlLaunchParameters_t* pParam)
{
	OnNewUrlLaunchParametersDelegate.Broadcast();
}

// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamVideo.h"

#include "SteamBridgeUtils.h"

USteamVideo::USteamVideo()
{
	/*OnBroadcastUploadStartCallback.Register(this, &USteamVideo::OnBroadcastUploadStart);
	OnBroadcastUploadStopCallback.Register(this, &USteamVideo::OnBroadcastUploadStop);*/
	OnGetOPFSettingsResultCallback.Register(this, &USteamVideo::OnGetOPFSettingsResult);
	OnGetVideoURLResultCallback.Register(this, &USteamVideo::OnGetVideoURLResult);
}

USteamVideo::~USteamVideo()
{
	/*OnBroadcastUploadStartCallback.Unregister();
	OnBroadcastUploadStopCallback.Unregister();*/
	OnGetOPFSettingsResultCallback.Unregister();
	OnGetVideoURLResultCallback.Unregister();
}

bool USteamVideo::GetOPFStringForApp(const int32 VideoAppID, FString& OPFString) const
{
	int32 TmpSize = 0;
	if (SteamVideo()->GetOPFStringForApp(VideoAppID, nullptr, &TmpSize))
	{
		TArray<char> TmpStr;
		bool bResult = SteamVideo()->GetOPFStringForApp(VideoAppID, TmpStr.GetData(), &TmpSize);
		OPFString = UTF8_TO_TCHAR(TmpStr.GetData());
		return bResult;
	}

	return false;
}

//void USteamVideo::OnBroadcastUploadStart(BroadcastUploadStart_t* pParam)
//{
//}
//
//void USteamVideo::OnBroadcastUploadStop(BroadcastUploadStop_t* pParam)
//{
//}

void USteamVideo::OnGetOPFSettingsResult(GetOPFSettingsResult_t* pParam)
{
	OnGetOPFSettingsResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_unVideoAppID);
}

void USteamVideo::OnGetVideoURLResult(GetVideoURLResult_t* pParam)
{
	OnGetVideoURLResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_unVideoAppID, UTF8_TO_TCHAR(pParam->m_rgchURL));
}

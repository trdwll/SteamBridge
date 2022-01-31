// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamRemoteStorage.h"

#include "SteamBridgeUtils.h"

USteamRemoteStorage::USteamRemoteStorage()
{
	OnRemoteStorageDownloadUGCResultCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageDownloadUGCResult);
	OnRemoteStorageFileReadAsyncCompleteCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageFileReadAsyncComplete);
	OnRemoteStorageFileShareResultCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageFileShareResult);
	OnRemoteStorageFileWriteAsyncCompleteCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageFileWriteAsyncComplete);
	OnRemoteStoragePublishedFileSubscribedCallback.Register(this, &USteamRemoteStorage::OnRemoteStoragePublishedFileSubscribed);
	OnRemoteStoragePublishedFileUnsubscribedCallback.Register(this, &USteamRemoteStorage::OnRemoteStoragePublishedFileUnsubscribed);
	OnRemoteStorageSubscribePublishedFileResultCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageSubscribePublishedFileResult);
	OnRemoteStorageUnsubscribePublishedFileResultCallback.Register(this, &USteamRemoteStorage::OnRemoteStorageUnsubscribePublishedFileResult);
}

USteamRemoteStorage::~USteamRemoteStorage()
{
	OnRemoteStorageDownloadUGCResultCallback.Unregister();
	OnRemoteStorageFileReadAsyncCompleteCallback.Unregister();
	OnRemoteStorageFileShareResultCallback.Unregister();
	OnRemoteStorageFileWriteAsyncCompleteCallback.Unregister();
	OnRemoteStoragePublishedFileSubscribedCallback.Unregister();
	OnRemoteStoragePublishedFileUnsubscribedCallback.Unregister();
	OnRemoteStorageSubscribePublishedFileResultCallback.Unregister();
	OnRemoteStorageUnsubscribePublishedFileResultCallback.Unregister();
}

int32 USteamRemoteStorage::FileRead(const FString& FileName, TArray<uint8>& Data, int32 DataToRead) const
{
	Data.SetNum(DataToRead);
	int32 result = SteamRemoteStorage()->FileRead(TCHAR_TO_UTF8(*FileName), Data.GetData(), Data.Num());
	Data.SetNum(result);
	return result;
}

bool USteamRemoteStorage::GetQuota(int64& TotalBytes, int64& AvailableBytes) const
{
	uint64 TmpTotal, TmpAvailable;
	bool bResult = SteamRemoteStorage()->GetQuota(&TmpTotal, &TmpAvailable);
	TotalBytes = TmpTotal;
	AvailableBytes = TmpAvailable;
	return bResult;
}

void USteamRemoteStorage::OnRemoteStorageDownloadUGCResult(RemoteStorageDownloadUGCResult_t* pParam)
{
	OnRemoteStorageDownloadUGCResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_hFile, pParam->m_nAppID, pParam->m_nSizeInBytes, UTF8_TO_TCHAR(pParam->m_pchFileName), pParam->m_ulSteamIDOwner);
}

void USteamRemoteStorage::OnRemoteStorageFileReadAsyncComplete(RemoteStorageFileReadAsyncComplete_t* pParam)
{
	OnRemoteStorageFileReadAsyncCompleteDelegate.Broadcast(pParam->m_hFileReadAsync, (ESteamResult)pParam->m_eResult, pParam->m_nOffset, pParam->m_cubRead);
}

void USteamRemoteStorage::OnRemoteStorageFileShareResult(RemoteStorageFileShareResult_t* pParam)
{
	OnRemoteStorageFileShareResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_hFile, UTF8_TO_TCHAR(pParam->m_rgchFilename));
}

void USteamRemoteStorage::OnRemoteStorageFileWriteAsyncComplete(RemoteStorageFileWriteAsyncComplete_t* pParam)
{
	OnRemoteStorageFileWriteAsyncCompleteDelegate.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamRemoteStorage::OnRemoteStoragePublishedFileSubscribed(RemoteStoragePublishedFileSubscribed_t* pParam)
{
	OnRemoteStoragePublishedFileSubscribedDelegate.Broadcast(pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamRemoteStorage::OnRemoteStoragePublishedFileUnsubscribed(RemoteStoragePublishedFileUnsubscribed_t* pParam)
{
	OnRemoteStoragePublishedFileUnsubscribedDelegate.Broadcast(pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamRemoteStorage::OnRemoteStorageSubscribePublishedFileResult(RemoteStorageSubscribePublishedFileResult_t* pParam)
{
	OnRemoteStorageSubscribePublishedFileResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId);
}

void USteamRemoteStorage::OnRemoteStorageUnsubscribePublishedFileResult(RemoteStorageUnsubscribePublishedFileResult_t* pParam)
{
	OnRemoteStorageUnsubscribePublishedFileResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId);
}

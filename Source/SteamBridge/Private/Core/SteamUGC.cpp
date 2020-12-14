// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUGC.h"

#include "SteamBridgeUtils.h"

USteamUGC::USteamUGC()
{
	OnAddAppDependencyResultCallback.Register(this, &USteamUGC::OnAddAppDependencyResult);
	OnAddUGCDependencyResultCallback.Register(this, &USteamUGC::OnAddUGCDependencyResult);
	OnCreateItemResultCallback.Register(this, &USteamUGC::OnCreateItemResult);
	OnDownloadItemResultCallback.Register(this, &USteamUGC::OnDownloadItemResult);
	OnGetAppDependenciesResultCallback.Register(this, &USteamUGC::OnGetAppDependenciesResult);
	OnDeleteItemResultCallback.Register(this, &USteamUGC::OnDeleteItemResult);
	OnGetUserItemVoteResultCallback.Register(this, &USteamUGC::OnGetUserItemVoteResult);
	OnItemInstalledCallback.Register(this, &USteamUGC::OnItemInstalled);
	OnRemoveAppDependencyResultCallback.Register(this, &USteamUGC::OnRemoveAppDependencyResult);
	OnRemoveUGCDependencyResultCallback.Register(this, &USteamUGC::OnRemoveUGCDependencyResult);
	OnSetUserItemVoteResultCallback.Register(this, &USteamUGC::OnSetUserItemVoteResult);
	OnStartPlaytimeTrackingResultCallback.Register(this, &USteamUGC::OnStartPlaytimeTrackingResult);
	OnSteamUGCQueryCompletedCallback.Register(this, &USteamUGC::OnSteamUGCQueryCompleted);
	OnStopPlaytimeTrackingResultCallback.Register(this, &USteamUGC::OnStopPlaytimeTrackingResult);
	OnSubmitItemUpdateResultCallback.Register(this, &USteamUGC::OnSubmitItemUpdateResult);
	OnUserFavoriteItemsListChangedCallback.Register(this, &USteamUGC::OnUserFavoriteItemsListChanged);
}

USteamUGC::~USteamUGC()
{
	OnAddAppDependencyResultCallback.Unregister();
	OnAddUGCDependencyResultCallback.Unregister();
	OnCreateItemResultCallback.Unregister();
	OnDownloadItemResultCallback.Unregister();
	OnGetAppDependenciesResultCallback.Unregister();
	OnDeleteItemResultCallback.Unregister();
	OnGetUserItemVoteResultCallback.Unregister();
	OnItemInstalledCallback.Unregister();
	OnRemoveAppDependencyResultCallback.Unregister();
	OnRemoveUGCDependencyResultCallback.Unregister();
	OnSetUserItemVoteResultCallback.Unregister();
	OnStartPlaytimeTrackingResultCallback.Unregister();
	OnSteamUGCQueryCompletedCallback.Unregister();
	OnStopPlaytimeTrackingResultCallback.Unregister();
	OnSubmitItemUpdateResultCallback.Unregister();
	OnUserFavoriteItemsListChangedCallback.Unregister();
}

bool USteamUGC::AddRequiredTagGroup(FUGCQueryHandle handle, const TArray<FString>& Tags) const
{
	TArray<char*> TmpTags;
	SteamParamStringArray_t TmpTagsArray;
	for (int32 i = 0; i < Tags.Num(); i++)
	{
		TmpTags.Add(TCHAR_TO_UTF8(*Tags[i]));
	}

	TmpTagsArray.m_ppStrings = (const char**)TmpTags.GetData();
	TmpTagsArray.m_nNumStrings = Tags.Num();

	return SteamUGC()->AddRequiredTagGroup(handle, &TmpTagsArray);
}

FUGCQueryHandle USteamUGC::CreateQueryAllUGCRequest(ESteamUGCQuery QueryType, ESteamUGCMatchingUGCType MatchingUGCTypeFileType, int32 CreatorAppID, int32 ConsumerAppID, int32 Page) const
{
	return SteamUGC()->CreateQueryAllUGCRequest((EUGCQuery)QueryType, (EUGCMatchingUGCType)MatchingUGCTypeFileType, CreatorAppID, ConsumerAppID, Page);
}

FUGCQueryHandle USteamUGC::CreateQueryUserUGCRequest(FAccountID AccountID, ESteamUserUGCList ListType, ESteamUGCMatchingUGCType MatchingUGCType, ESteamUserUGCListSortOrder SortOrder, int32 CreatorAppID, int32 ConsumerAppID, int32 Page) const
{
	return SteamUGC()->CreateQueryUserUGCRequest(AccountID, (EUserUGCList)ListType, (EUGCMatchingUGCType)MatchingUGCType, (EUserUGCListSortOrder)SortOrder, CreatorAppID, ConsumerAppID, Page);
}

bool USteamUGC::GetItemInstallInfo(FPublishedFileId PublishedFileID, int64& SizeOnDisk, FString& FolderName, int32 FolderSize, int32& TimeStamp) const
{
	TArray<char> TmpData;
	TmpData.SetNum(FolderSize);
	bool bResult = SteamUGC()->GetItemInstallInfo(PublishedFileID, (uint64*)&SizeOnDisk, TmpData.GetData(), TmpData.Num(), (uint32*)&TimeStamp);
	FolderName = UTF8_TO_TCHAR(TmpData.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCAdditionalPreview(FUGCQueryHandle handle, int32 index, int32 previewIndex, FString& URLOrVideoID, int32 URLSize, FString& OriginalFileName, int32 OriginalFileNameSize, ESteamItemPreviewType& PreviewType) const
{
	TArray<char> TmpStr, TmpStr2;
	TmpStr.SetNum(URLSize);
	TmpStr2.SetNum(OriginalFileNameSize);
	bool bResult = SteamUGC()->GetQueryUGCAdditionalPreview(handle, index, previewIndex, TmpStr.GetData(), TmpStr.Num(), TmpStr2.GetData(), TmpStr2.Num(), (EItemPreviewType*)&PreviewType);
	URLOrVideoID = UTF8_TO_TCHAR(TmpStr.GetData());
	OriginalFileName = UTF8_TO_TCHAR(TmpStr2.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCChildren(FUGCQueryHandle handle, int32 index, TArray<FPublishedFileId>& PublishedFileIDs, int32 MaxEntries) const
{
	TArray<PublishedFileId_t> TmpData;
	TmpData.SetNum(MaxEntries);
	bool bResult = SteamUGC()->GetQueryUGCChildren(handle, index, TmpData.GetData(), TmpData.Num());
	for (int32 i = 0; i < TmpData.Num(); i++)
	{
		PublishedFileIDs.Add(TmpData[i]);
	}
	return bResult;
}

bool USteamUGC::GetQueryUGCKeyValueTag(FUGCQueryHandle handle, int32 index, int32 keyValueTagIndex, FString& Key, int32 KeySize, FString& Value, int32 ValueSize) const
{
	TArray<char> TmpKey, TmpValue;
	TmpKey.SetNum(KeySize);
	TmpValue.SetNum(ValueSize);
	bool bResult = SteamUGC()->GetQueryUGCKeyValueTag(handle, index, keyValueTagIndex, TmpKey.GetData(), TmpKey.Num(), TmpValue.GetData(), TmpValue.Num());
	Key = UTF8_TO_TCHAR(TmpKey.GetData());
	Value = UTF8_TO_TCHAR(TmpValue.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCMetadata(FUGCQueryHandle handle, int32 index, FString& Metadata, int32 Metadatasize) const
{
	TArray<char> TmpStr;
	bool bResult = SteamUGC()->GetQueryUGCMetadata(handle, index, TmpStr.GetData(), TmpStr.Num());
	Metadata = UTF8_TO_TCHAR(TmpStr.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCPreviewURL(FUGCQueryHandle handle, int32 index, FString& URL, int32 URLSize) const
{
	TArray<char> TmpUrl;
	TmpUrl.SetNum(URLSize);
	bool bResult = SteamUGC()->GetQueryUGCPreviewURL(handle, index, TmpUrl.GetData(), TmpUrl.Num());
	URL = UTF8_TO_TCHAR(TmpUrl.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCResult(FUGCQueryHandle handle, int32 index, FSteamUGCDetails& Details) const
{
	SteamUGCDetails_t TmpDetails;
	bool bResult = SteamUGC()->GetQueryUGCResult(handle, index, &TmpDetails);
	Details = TmpDetails;
	return bResult;
}

int32 USteamUGC::GetSubscribedItems(TArray<FPublishedFileId>& PublishedFileIDs, int32 MaxEntries) const
{
	TArray<PublishedFileId_t> TmpData;
	int32 result = SteamUGC()->GetSubscribedItems(TmpData.GetData(), MaxEntries);
	for (int32 i = 0; i < result; i++)
	{
		PublishedFileIDs.Add(TmpData[i]);
	}

	return result;
}

bool USteamUGC::SetItemTags(FUGCUpdateHandle UpdateHandle, const TArray<FString>& Tags) const
{
	TArray<char*> TmpTags;
	SteamParamStringArray_t TmpTagsArray;
	for (int32 i = 0; i < Tags.Num(); i++)
	{
		TmpTags.Add(TCHAR_TO_UTF8(*Tags[i]));
	}

	TmpTagsArray.m_ppStrings = (const char**)TmpTags.GetData();
	TmpTagsArray.m_nNumStrings = Tags.Num();
	return SteamUGC()->SetItemTags(UpdateHandle, &TmpTagsArray);
}

void USteamUGC::OnAddAppDependencyResult(AddAppDependencyResult_t* pParam)
{
	m_OnAddAppDependencyResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamUGC::OnAddUGCDependencyResult(AddUGCDependencyResult_t* pParam)
{
	m_OnAddUGCDependencyResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nChildPublishedFileId);
}

void USteamUGC::OnCreateItemResult(CreateItemResult_t* pParam)
{
	m_OnCreateItemResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_bUserNeedsToAcceptWorkshopLegalAgreement);
}

void USteamUGC::OnDownloadItemResult(DownloadItemResult_t* pParam)
{
	m_OnDownloadItemResult.Broadcast(pParam->m_unAppID, pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnGetAppDependenciesResult(GetAppDependenciesResult_t* pParam)
{
	TArray<int32> AppIDs;
	for (int32 i = 0; i < 32; i++)
	{
		AppIDs.Add(pParam->m_rgAppIDs[i]);
	}

	m_OnGetAppDependenciesResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, AppIDs, AppIDs.Num(), pParam->m_nTotalNumAppDependencies);
}

void USteamUGC::OnDeleteItemResult(DeleteItemResult_t* pParam)
{
	m_OnDeleteItemResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId);
}

void USteamUGC::OnGetUserItemVoteResult(GetUserItemVoteResult_t* pParam)
{
	m_OnGetUserItemVoteResult.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bVotedUp, pParam->m_bVotedDown, pParam->m_bVoteSkipped);
}

void USteamUGC::OnItemInstalled(ItemInstalled_t* pParam)
{
	m_OnItemInstalled.Broadcast(pParam->m_unAppID, pParam->m_nPublishedFileId);
}

void USteamUGC::OnRemoveAppDependencyResult(RemoveAppDependencyResult_t* pParam)
{
	m_OnRemoveAppDependencyResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamUGC::OnRemoveUGCDependencyResult(RemoveUGCDependencyResult_t* pParam)
{
	m_OnRemoveUGCDependencyResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nChildPublishedFileId);
}

void USteamUGC::OnSetUserItemVoteResult(SetUserItemVoteResult_t* pParam)
{
	m_OnSetUserItemVoteResult.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bVoteUp);
}

void USteamUGC::OnStartPlaytimeTrackingResult(StartPlaytimeTrackingResult_t* pParam)
{
	m_OnStartPlaytimeTrackingResult.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnSteamUGCQueryCompleted(SteamUGCQueryCompleted_t* pParam)
{
	m_OnSteamUGCQueryCompleted.Broadcast(pParam->m_handle, (ESteamResult)pParam->m_eResult, pParam->m_unNumResultsReturned, pParam->m_unTotalMatchingResults, pParam->m_bCachedData);
}

void USteamUGC::OnStopPlaytimeTrackingResult(StopPlaytimeTrackingResult_t* pParam)
{
	m_OnStopPlaytimeTrackingResult.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* pParam)
{
	m_OnSubmitItemUpdateResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_bUserNeedsToAcceptWorkshopLegalAgreement);
}

void USteamUGC::OnUserFavoriteItemsListChanged(UserFavoriteItemsListChanged_t* pParam)
{
	m_OnUserFavoriteItemsListChanged.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bWasAddRequest);
}

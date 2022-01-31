// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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

bool USteamUGC::AddRequiredTagGroup(const FUGCQueryHandle handle, const TArray<FString>& Tags) const
{
	TArray<char*> TmpTags;
	TmpTags.Reserve(Tags.Num());
	SteamParamStringArray_t TmpTagsArray;
	for (int32 i = 0; i < Tags.Num(); i++)
	{
		TmpTags.Add(TCHAR_TO_UTF8(*Tags[i]));
	}

	TmpTagsArray.m_ppStrings = (const char**)TmpTags.GetData();
	TmpTagsArray.m_nNumStrings = Tags.Num();

	return SteamUGC()->AddRequiredTagGroup(handle, &TmpTagsArray);
}

FUGCQueryHandle USteamUGC::CreateQueryAllUGCRequest(const ESteamUGCQuery QueryType, const ESteamUGCMatchingUGCType MatchingUGCTypeFileType, const int32 CreatorAppID, const int32 ConsumerAppID, const int32 Page) const
{
	return SteamUGC()->CreateQueryAllUGCRequest((EUGCQuery)QueryType, (EUGCMatchingUGCType)MatchingUGCTypeFileType, CreatorAppID, ConsumerAppID, Page);
}

FUGCQueryHandle USteamUGC::CreateQueryUserUGCRequest(const FAccountID AccountID, const ESteamUserUGCList ListType, ESteamUGCMatchingUGCType MatchingUGCType, ESteamUserUGCListSortOrder SortOrder, const int32 CreatorAppID, const int32 ConsumerAppID, const int32 Page) const
{
	return SteamUGC()->CreateQueryUserUGCRequest(AccountID, (EUserUGCList)ListType, (EUGCMatchingUGCType)MatchingUGCType, (EUserUGCListSortOrder)SortOrder, CreatorAppID, ConsumerAppID, Page);
}


bool USteamUGC::GetItemInstallInfo(const FPublishedFileId PublishedFileID, int64& SizeOnDisk, FString& FolderName, const int32 FolderSize, FDateTime& TimeStamp) const
{
	uint32 TmpTime;
	TArray<char> TmpData;
	TmpData.SetNum(FolderSize);
	bool bResult = SteamUGC()->GetItemInstallInfo(PublishedFileID, (uint64*)&SizeOnDisk, TmpData.GetData(), TmpData.Num(), &TmpTime);
	FolderName = UTF8_TO_TCHAR(TmpData.GetData());
	TimeStamp = FDateTime::FromUnixTimestamp(TmpTime);
	return bResult;
}

bool USteamUGC::GetQueryUGCAdditionalPreview(const FUGCQueryHandle handle, const int32 index, const int32 previewIndex, FString& URLOrVideoID, const int32 URLSize, FString& OriginalFileName, int32 OriginalFileNameSize, ESteamItemPreviewType& PreviewType) const
{
	TArray<char> TmpStr, TmpStr2;
	TmpStr.SetNum(URLSize);
	TmpStr2.SetNum(OriginalFileNameSize);
	bool bResult = SteamUGC()->GetQueryUGCAdditionalPreview(handle, index, previewIndex, TmpStr.GetData(), TmpStr.Num(), TmpStr2.GetData(), TmpStr2.Num(), (EItemPreviewType*)&PreviewType);
	URLOrVideoID = UTF8_TO_TCHAR(TmpStr.GetData());
	OriginalFileName = UTF8_TO_TCHAR(TmpStr2.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCChildren(const FUGCQueryHandle handle, const int32 index, TArray<FPublishedFileId>& PublishedFileIDs, const int32 MaxEntries) const
{
	TArray<PublishedFileId_t> TmpData;
	TmpData.SetNum(MaxEntries);
	bool bResult = SteamUGC()->GetQueryUGCChildren(handle, index, TmpData.GetData(), TmpData.Num());
	for (const auto& Entry : TmpData)
	{
		PublishedFileIDs.Add(Entry);
	}
	return bResult;
}

bool USteamUGC::GetQueryUGCKeyValueTag(const FUGCQueryHandle handle, const int32 index, const int32 keyValueTagIndex, FString& Key, const int32 KeySize, FString& Value, const int32 ValueSize) const
{
	TArray<char> TmpKey, TmpValue;
	TmpKey.SetNum(KeySize);
	TmpValue.SetNum(ValueSize);
	bool bResult = SteamUGC()->GetQueryUGCKeyValueTag(handle, index, keyValueTagIndex, TmpKey.GetData(), TmpKey.Num(), TmpValue.GetData(), TmpValue.Num());
	Key = UTF8_TO_TCHAR(TmpKey.GetData());
	Value = UTF8_TO_TCHAR(TmpValue.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCMetadata(const FUGCQueryHandle handle, const int32 index, FString& Metadata, const int32 Metadatasize) const
{
	TArray<char> TmpStr;
	bool bResult = SteamUGC()->GetQueryUGCMetadata(handle, index, TmpStr.GetData(), TmpStr.Num());
	Metadata = UTF8_TO_TCHAR(TmpStr.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCPreviewURL(const FUGCQueryHandle handle, const int32 index, FString& URL, const int32 URLSize) const
{
	TArray<char> TmpUrl;
	TmpUrl.SetNum(URLSize);
	bool bResult = SteamUGC()->GetQueryUGCPreviewURL(handle, index, TmpUrl.GetData(), TmpUrl.Num());
	URL = UTF8_TO_TCHAR(TmpUrl.GetData());
	return bResult;
}

bool USteamUGC::GetQueryUGCResult(const FUGCQueryHandle handle, const int32 index, FSteamUGCDetails& Details) const
{
	SteamUGCDetails_t TmpDetails;
	bool bResult = SteamUGC()->GetQueryUGCResult(handle, index, &TmpDetails);
	Details = TmpDetails;
	return bResult;
}

int32 USteamUGC::GetSubscribedItems(TArray<FPublishedFileId>& PublishedFileIDs, const int32 MaxEntries) const
{
	TArray<PublishedFileId_t> TmpData;
	int32 result = SteamUGC()->GetSubscribedItems(TmpData.GetData(), MaxEntries);
	for (int32 i = 0; i < result; i++)
	{
		PublishedFileIDs.Add(TmpData[i]);
	}

	return result;
}

bool USteamUGC::SetItemTags(const FUGCUpdateHandle UpdateHandle, const TArray<FString>& Tags) const
{
	TArray<char*> TmpTags;
	TmpTags.Reserve(Tags.Num());
	SteamParamStringArray_t TmpTagsArray;
	for (const auto& Tag : Tags)
	{
		TmpTags.Add(TCHAR_TO_UTF8(*Tag));
	}

	TmpTagsArray.m_ppStrings = (const char**)TmpTags.GetData();
	TmpTagsArray.m_nNumStrings = Tags.Num();
	return SteamUGC()->SetItemTags(UpdateHandle, &TmpTagsArray);
}

void USteamUGC::OnAddAppDependencyResult(AddAppDependencyResult_t* pParam)
{
	OnAddAppDependencyResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamUGC::OnAddUGCDependencyResult(AddUGCDependencyResult_t* pParam)
{
	OnAddUGCDependencyResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nChildPublishedFileId);
}

void USteamUGC::OnCreateItemResult(CreateItemResult_t* pParam)
{
	OnCreateItemResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_bUserNeedsToAcceptWorkshopLegalAgreement);
}

void USteamUGC::OnDownloadItemResult(DownloadItemResult_t* pParam)
{
	OnDownloadItemResultDelegate.Broadcast(pParam->m_unAppID, pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnGetAppDependenciesResult(GetAppDependenciesResult_t* pParam)
{
	TArray<int32> AppIDs;
	AppIDs.Reserve(32);
	for (int32 i = 0; i < 32; i++)
	{
		AppIDs.Add(pParam->m_rgAppIDs[i]);
	}

	OnGetAppDependenciesResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, AppIDs, AppIDs.Num(), pParam->m_nTotalNumAppDependencies);
}

void USteamUGC::OnDeleteItemResult(DeleteItemResult_t* pParam)
{
	OnDeleteItemResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId);
}

void USteamUGC::OnGetUserItemVoteResult(GetUserItemVoteResult_t* pParam)
{
	OnGetUserItemVoteResultDelegate.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bVotedUp, pParam->m_bVotedDown, pParam->m_bVoteSkipped);
}

void USteamUGC::OnItemInstalled(ItemInstalled_t* pParam)
{
	OnItemInstalledDelegate.Broadcast(pParam->m_unAppID, pParam->m_nPublishedFileId);
}

void USteamUGC::OnRemoveAppDependencyResult(RemoveAppDependencyResult_t* pParam)
{
	OnRemoveAppDependencyResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nAppID);
}

void USteamUGC::OnRemoveUGCDependencyResult(RemoveUGCDependencyResult_t* pParam)
{
	OnRemoveUGCDependencyResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_nPublishedFileId, pParam->m_nChildPublishedFileId);
}

void USteamUGC::OnSetUserItemVoteResult(SetUserItemVoteResult_t* pParam)
{
	OnSetUserItemVoteResultDelegate.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bVoteUp);
}

void USteamUGC::OnStartPlaytimeTrackingResult(StartPlaytimeTrackingResult_t* pParam)
{
	OnStartPlaytimeTrackingResultDelegate.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnSteamUGCQueryCompleted(SteamUGCQueryCompleted_t* pParam)
{
	OnSteamUGCQueryCompletedDelegate.Broadcast(pParam->m_handle, (ESteamResult)pParam->m_eResult, pParam->m_unNumResultsReturned, pParam->m_unTotalMatchingResults, pParam->m_bCachedData);
}

void USteamUGC::OnStopPlaytimeTrackingResult(StopPlaytimeTrackingResult_t* pParam)
{
	OnStopPlaytimeTrackingResultDelegate.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUGC::OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* pParam)
{
	OnSubmitItemUpdateResultDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_bUserNeedsToAcceptWorkshopLegalAgreement);
}

void USteamUGC::OnUserFavoriteItemsListChanged(UserFavoriteItemsListChanged_t* pParam)
{
	OnUserFavoriteItemsListChangedDelegate.Broadcast(pParam->m_nPublishedFileId, (ESteamResult)pParam->m_eResult, pParam->m_bWasAddRequest);
}

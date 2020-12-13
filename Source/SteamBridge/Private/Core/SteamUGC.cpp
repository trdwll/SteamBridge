// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUGC.h"

#include "SteamBridgeUtils.h"

USteamUGC::USteamUGC()
{
}

USteamUGC::~USteamUGC()
{
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

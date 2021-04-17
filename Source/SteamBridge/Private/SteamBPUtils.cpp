// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamBPUtils.h"

FSteamID USteamBPUtils::MakeSteamID(const FString& SteamID)
{
	uint64 TmpSteamID;
	LexFromString(TmpSteamID, *SteamID);
	return {TmpSteamID};
}

FPublishedFileId USteamBPUtils::MakePublishedFileId(const FString& FileID)
{
	uint64 TmpFileID;
	LexFromString(TmpFileID, *FileID);
	return { TmpFileID };
}

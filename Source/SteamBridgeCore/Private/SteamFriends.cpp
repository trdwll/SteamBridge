// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamFriends.h"

#include "Steam.h"

USteamFriends::USteamFriends()
{
	OnAvatarImageLoadedCallback.Register(this, &USteamFriends::OnAvatarImageLoaded);
	OnClanOfficerListResponseCallback.Register(this, &USteamFriends::OnClanOfficerListResponse);
	OnDownloadClanActivityCountsResultCallback.Register(this, &USteamFriends::OnDownloadClanActivityCountsResult);
	OnFriendRichPresenceUpdateCallback.Register(this, &USteamFriends::OnFriendRichPresenceUpdate);
	OnFriendsEnumerateFollowingListCallback.Register(this, &USteamFriends::OnFriendsEnumerateFollowingList);
	OnFriendsGetFollowerCountCallback.Register(this, &USteamFriends::OnFriendsGetFollowerCount);
	OnFriendsIsFollowingCallback.Register(this, &USteamFriends::OnFriendsIsFollowing);
	OnGameConnectedChatJoinCallback.Register(this, &USteamFriends::OnGameConnectedChatJoin);
	OnGameConnectedChatLeaveCallback.Register(this, &USteamFriends::OnGameConnectedChatLeave);
	OnGameConnectedClanChatMsgCallback.Register(this, &USteamFriends::OnGameConnectedClanChatMsg);
	OnGameConnectedFriendChatMsgCallback.Register(this, &USteamFriends::OnGameConnectedFriendChatMsg);
	OnGameLobbyJoinRequestedCallback.Register(this, &USteamFriends::OnGameLobbyJoinRequested);
	OnGameOverlayActivatedCallback.Register(this, &USteamFriends::OnGameOverlayActivated);
	OnGameRichPresenceJoinRequestedCallback.Register(this, &USteamFriends::OnGameRichPresenceJoinRequested);
	OnGameServerChangeRequestedCallback.Register(this, &USteamFriends::OnGameServerChangeRequested);
	OnJoinClanChatRoomCompletionResultCallback.Register(this, &USteamFriends::OnJoinClanChatRoomCompletionResult);
	OnPersonaStateChangeCallback.Register(this, &USteamFriends::OnPersonaStateChange);
	OnSetPersonaNameResponseCallback.Register(this, &USteamFriends::OnSetPersonaNameResponse);
}

USteamFriends::~USteamFriends()
{
	OnAvatarImageLoadedCallback.Unregister();
	OnClanOfficerListResponseCallback.Unregister();
	OnDownloadClanActivityCountsResultCallback.Unregister();
	OnFriendRichPresenceUpdateCallback.Unregister();
	OnFriendsEnumerateFollowingListCallback.Unregister();
	OnFriendsGetFollowerCountCallback.Unregister();
	OnFriendsIsFollowingCallback.Unregister();
	OnGameConnectedChatJoinCallback.Unregister();
	OnGameConnectedChatLeaveCallback.Unregister();
	OnGameConnectedClanChatMsgCallback.Unregister();
	OnGameConnectedFriendChatMsgCallback.Unregister();
	OnGameLobbyJoinRequestedCallback.Unregister();
	OnGameOverlayActivatedCallback.Unregister();
	OnGameRichPresenceJoinRequestedCallback.Unregister();
	OnGameServerChangeRequestedCallback.Unregister();
	OnJoinClanChatRoomCompletionResultCallback.Unregister();
	OnPersonaStateChangeCallback.Unregister();
	OnSetPersonaNameResponseCallback.Unregister();
}

void USteamFriends::ActivateGameOverlay(const ESteamGameOverlayTypes OverlayType)
{
	const FString Str = USteamBridgeUtils::GetEnumValueAsStringParsed(USteamBridgeUtils::GetEnumValueAsString<ESteamGameOverlayTypes>("ESteamGameOverlayTypes", OverlayType));
	SteamFriends()->ActivateGameOverlay(TCHAR_TO_ANSI(*Str));
}

void USteamFriends::ActivateGameOverlayToUser(const ESteamGameUserOverlayTypes OverlayType, FSteamID SteamID)
{
	const FString Str = USteamBridgeUtils::GetEnumValueAsStringParsed(USteamBridgeUtils::GetEnumValueAsString<ESteamGameUserOverlayTypes>("ESteamGameUserOverlayTypes", OverlayType));
	SteamFriends()->ActivateGameOverlayToUser(TCHAR_TO_ANSI(*Str), SteamID.Value);
}

FSteamAPICall USteamFriends::DownloadClanActivityCounts(TArray<FSteamID>& SteamClanIDs, int32 ClansToRequest) const
{
	TArray<CSteamID> TmpArray;
	uint64 res = SteamFriends()->DownloadClanActivityCounts(TmpArray.GetData(), ClansToRequest);

	for (int32 i = 0; i < ClansToRequest; i++)
	{
		SteamClanIDs.Add(TmpArray[i].ConvertToUint64());
	}

	return res;
}


int32 USteamFriends::GetClanChatMessage(FSteamID SteamIDClanChat, int32 MessageID, FString& Message, ESteamChatEntryType& ChatEntryType, FSteamID& SteamIDChatter)
{
	EChatEntryType TmpEntryType;
	TArray<char> TmpMessage;
	TmpMessage.SetNum(MAX_int32);
	CSteamID TmpSteamID;
	int32 res = SteamFriends()->GetClanChatMessage(SteamIDClanChat.Value, MessageID, TmpMessage.GetData(), MAX_int32, &TmpEntryType, &TmpSteamID);
	if (res < 0)
	{
		return -1;
	}
	SteamIDChatter = TmpSteamID.ConvertToUint64();
	TmpMessage.SetNum(res);
	Message = UTF8_TO_TCHAR(TmpMessage.GetData());
	ChatEntryType = (ESteamChatEntryType)TmpEntryType;

	return res;
}

FSteamID USteamFriends::GetFriendByIndex(int32 FriendIndex, const TArray<ESteamFriendFlags>& FriendFlags) const
{
	int32 flags = FriendFlags.Contains(ESteamFriendFlags::All) ? 0xFFFF : 0;

	if (flags == 0)
	{
		for (int32 i = 0; i < FriendFlags.Num(); i++)
		{
			flags |= 1 << (int32)FriendFlags[i];
		}
	}

	return SteamFriends()->GetFriendByIndex(FriendIndex, flags).ConvertToUint64();
}

int32 USteamFriends::GetFriendCount(const TArray<ESteamFriendFlags>& FriendFlags) const
{
	int32 flags = FriendFlags.Contains(ESteamFriendFlags::All) ? 0xFFFF : 0;

	if (flags == 0)
	{
		for (int32 i = 0; i < FriendFlags.Num(); i++)
		{
			flags |= 1 << (int32)FriendFlags[i];
		}
	}

	return SteamFriends()->GetFriendCount(flags);
}

bool USteamFriends::GetFriendGamePlayed(FSteamID SteamIDFriend, FSteamID& GameID, FString& GameIP, int32& GamePort, int32& QueryPort, FSteamID& SteamIDLobby)
{
	FriendGameInfo_t InGameInfoStruct;
	bool bResult = SteamFriends()->GetFriendGamePlayed(SteamIDFriend.Value, &InGameInfoStruct);
	GameID = InGameInfoStruct.m_gameID.ToUint64();
	GameIP = USteamBridgeUtils::ConvertIPToString(InGameInfoStruct.m_unGameIP);
	GamePort = InGameInfoStruct.m_usGamePort;
	QueryPort = InGameInfoStruct.m_usQueryPort;
	SteamIDLobby = InGameInfoStruct.m_steamIDLobby.ConvertToUint64();
	return bResult;
}

int32 USteamFriends::GetFriendMessage(FSteamID SteamIDFriend, int32 MessageIndex, FString& Message, ESteamChatEntryType& ChatEntryType)
{
	EChatEntryType TmpEntryType;
	TArray<char> TmpMessage;
	TmpMessage.SetNum(MAX_int32);
	int32 res = SteamFriends()->GetFriendMessage(SteamIDFriend.Value, MessageIndex, TmpMessage.GetData(), MAX_int32, &TmpEntryType);
	if (res < 0)
	{
		return 0;
	}
	TmpMessage.SetNum(res);
	Message = UTF8_TO_TCHAR(TmpMessage.GetData());
	ChatEntryType = (ESteamChatEntryType)TmpEntryType;
	return res;
}

void USteamFriends::GetFriendsGroupMembersList(FSteamFriendsGroupID FriendsGroupID, TArray<FSteamID>& MemberSteamIDs)
{
	int32 count = GetFriendsGroupMembersCount(FriendsGroupID);
	if (count < 0)
	{
		return;
	}

	TArray<CSteamID> TmpArray;
	SteamFriends()->GetFriendsGroupMembersList(FriendsGroupID.Value, TmpArray.GetData(), count);
	for (int32 i = 0; i < count; i++)
	{
		MemberSteamIDs.Add(TmpArray[i].ConvertToUint64());
	}
}

UTexture2D* USteamFriends::GetFriendAvatar(FSteamID SteamIDFriend, ESteamAvatarSize AvatarSize) const
{
	int32 Avatar;
	switch (AvatarSize)
	{
	case ESteamAvatarSize::Small:
		Avatar = SteamFriends()->GetSmallFriendAvatar(SteamIDFriend.Value);
		break;
	case ESteamAvatarSize::Medium:
		Avatar = SteamFriends()->GetMediumFriendAvatar(SteamIDFriend.Value);
		break;
	case ESteamAvatarSize::Large:
		Avatar = SteamFriends()->GetLargeFriendAvatar(SteamIDFriend.Value);
		break;
	}

	uint32 Width, Height;
	SteamUtils()->GetImageSize(Avatar, &Width, &Height);
	if (Width > 0 && Height > 0)
	{
		UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
		uint8* AvatarRGBA = new uint8[Width * Height * 4];
		SteamUtils()->GetImageRGBA(Avatar, (uint8*)AvatarRGBA, 4 * Height * Width);
		for (uint32 i = 0; i < (Width * Height * 4); i += 4)
		{
			uint8 Temp = AvatarRGBA[i + 0];
			AvatarRGBA[i + 0] = AvatarRGBA[i + 2];
			AvatarRGBA[i + 2] = Temp;
		}
		uint8* MipData = (uint8*)AvatarTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(MipData, (void*)AvatarRGBA, Height * Width * 4);
		AvatarTexture->PlatformData->Mips[0].BulkData.Unlock();
		AvatarTexture->PlatformData->SetNumSlices(1);
		AvatarTexture->NeverStream = true;
		AvatarTexture->UpdateResource();
		delete[] AvatarRGBA;

		return AvatarTexture;
	}

	return nullptr;
}

TArray<ESteamUserRestrictions> USteamFriends::GetUserRestrictions() const
{
	TArray<ESteamUserRestrictions> TmpArray;
	uint32 flags = SteamFriends()->GetUserRestrictions();
	for (int i = 0; i < 32; i++)
	{
		if (flags & 1 << i)
		{
			TmpArray.Add((ESteamUserRestrictions)i);
		}
	}
	return TmpArray;
}

bool USteamFriends::HasFriend(FSteamID SteamIDFriend, const TArray<ESteamFriendFlags>& FriendFlags)
{
	int32 flags = FriendFlags.Contains(ESteamFriendFlags::All) ? 0xFFFF : 0;

	if (flags == 0)
	{
		for (int32 i = 0; i < FriendFlags.Num(); i++)
		{
			flags |= 1 << (int32)FriendFlags[i];
		}
	}

	return SteamFriends()->HasFriend(SteamIDFriend.Value, flags);
}

void USteamFriends::OnAvatarImageLoaded(AvatarImageLoaded_t* pParam)
{
	m_OnAvatarImageLoaded.Broadcast(pParam->m_steamID.ConvertToUint64(), pParam->m_iImage, pParam->m_iWide, pParam->m_iTall);
}

void USteamFriends::OnClanOfficerListResponse(ClanOfficerListResponse_t* pParam)
{
	m_OnClanOfficerListResponse.Broadcast(pParam->m_steamIDClan.ConvertToUint64(), pParam->m_cOfficers, pParam->m_bSuccess == 1);
}

void USteamFriends::OnDownloadClanActivityCountsResult(DownloadClanActivityCountsResult_t* pParam)
{
	m_OnDownloadClanActivityCountsResult.Broadcast(pParam->m_bSuccess);
}

void USteamFriends::OnFriendRichPresenceUpdate(FriendRichPresenceUpdate_t* pParam)
{
	m_OnFriendRichPresenceUpdate.Broadcast(pParam->m_steamIDFriend.ConvertToUint64(), pParam->m_nAppID);
}

void USteamFriends::OnFriendsEnumerateFollowingList(FriendsEnumerateFollowingList_t* pParam)
{
#if 0
	TArray<FSteamID> TmpArray;

	for (int32 i = 0; i < pParam->m_nResultsReturned; i++)
	{
		TmpArray.Add(pParam->m_rgSteamID[i].ConvertToUint64());
	}

	m_OnFriendsEnumerateFollowingList.Broadcast((ESteamResult)pParam->m_eResult, TmpArray, pParam->m_nResultsReturned, pParam->m_nTotalResultCount);
#endif // 0
}

void USteamFriends::OnFriendsGetFollowerCount(FriendsGetFollowerCount_t* pParam)
{
	m_OnFriendsGetFollowerCount.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_steamID.ConvertToUint64(), pParam->m_nCount);
}

void USteamFriends::OnFriendsIsFollowing(FriendsIsFollowing_t* pParam)
{
	m_OnFriendsIsFollowing.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_steamID.ConvertToUint64(), pParam->m_bIsFollowing);
}

void USteamFriends::OnGameConnectedChatJoin(GameConnectedChatJoin_t* pParam)
{
	m_OnGameConnectedChatJoin.Broadcast(pParam->m_steamIDClanChat.ConvertToUint64(), pParam->m_steamIDUser.ConvertToUint64());
}

void USteamFriends::OnGameConnectedChatLeave(GameConnectedChatLeave_t* pParam)
{
	m_OnGameConnectedChatLeave.Broadcast(pParam->m_steamIDClanChat.ConvertToUint64(), pParam->m_steamIDUser.ConvertToUint64(), pParam->m_bKicked, pParam->m_bDropped);
}

void USteamFriends::OnGameConnectedClanChatMsg(GameConnectedClanChatMsg_t* pParam)
{
	m_OnGameConnectedClanChatMsg.Broadcast(pParam->m_steamIDClanChat.ConvertToUint64(), pParam->m_steamIDUser.ConvertToUint64(), pParam->m_iMessageID);
}

void USteamFriends::OnGameConnectedFriendChatMsg(GameConnectedFriendChatMsg_t* pParam)
{
	m_OnGameConnectedFriendChatMsg.Broadcast(pParam->m_steamIDUser.ConvertToUint64(), pParam->m_iMessageID);
}

void USteamFriends::OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* pParam)
{
	m_OnGameLobbyJoinRequested.Broadcast(pParam->m_steamIDLobby.ConvertToUint64(), pParam->m_steamIDFriend.ConvertToUint64());
}

void USteamFriends::OnGameOverlayActivated(GameOverlayActivated_t* pParam)
{
	m_OnGameOverlayActivated.Broadcast(pParam->m_bActive == 1);
}

void USteamFriends::OnGameRichPresenceJoinRequested(GameRichPresenceJoinRequested_t* pParam)
{
	m_OnGameRichPresenceJoinRequested.Broadcast(pParam->m_steamIDFriend.ConvertToUint64(), UTF8_TO_TCHAR(pParam->m_rgchConnect));
}

void USteamFriends::OnGameServerChangeRequested(GameServerChangeRequested_t* pParam)
{
	m_OnGameServerChangeRequested.Broadcast(UTF8_TO_TCHAR(pParam->m_rgchServer), UTF8_TO_TCHAR(pParam->m_rgchPassword));
}

void USteamFriends::OnJoinClanChatRoomCompletionResult(JoinClanChatRoomCompletionResult_t* pParam)
{
	m_OnJoinClanChatRoomCompletionResult.Broadcast(pParam->m_steamIDClanChat.ConvertToUint64(), (ESteamChatRoomEnterResponse)pParam->m_eChatRoomEnterResponse);
}

void USteamFriends::OnPersonaStateChange(PersonaStateChange_t* pParam)
{
	m_OnPersonaStateChange.Broadcast(pParam->m_ulSteamID, static_cast<ESteamPersonaChange>((uint8)pParam->m_nChangeFlags));
}

void USteamFriends::OnSetPersonaNameResponse(SetPersonaNameResponse_t* pParam)
{
	m_OnSetPersonaNameResponse.Broadcast(pParam->m_bSuccess, pParam->m_bLocalSuccess, (ESteamResult)pParam->m_result);
}

// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamFriends.h"

#include "Steam.h"

USteamFriends::USteamFriends()
{
	OnSteamPersonaStateChangeCallback.Register(this, &USteamFriends::OnSteamPersonaStateChange);
}

USteamFriends::~USteamFriends()
{
	OnSteamPersonaStateChangeCallback.Unregister();
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





FString USteamFriends::GetPersonaName() const
{
	return SteamFriends()->GetPersonaName();
}

bool USteamFriends::RequestUserInformation(FSteamID SteamID, bool bRequireNameOnly)
{
	return SteamFriends()->RequestUserInformation(SteamID.Value, bRequireNameOnly);
}

void USteamFriends::OnSteamPersonaStateChange(PersonaStateChange_t* pParam)
{
	m_SteamPersonaStateChangeCallback.Broadcast(pParam->m_ulSteamID, static_cast<ESteamPersonaChange>((uint8)pParam->m_nChangeFlags));
}

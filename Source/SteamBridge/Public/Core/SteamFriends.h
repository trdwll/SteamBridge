// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamFriends.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAvatarImageLoadedDelegate, FSteamID, SteamID, int32, ImageHandle, int32, Width, int32, Height);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnClanOfficerListResponseDelegate, FSteamID, SteamID, int32, OfficersCount, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDownloadClanActivityCountsResultDelegate, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFriendRichPresenceUpdateDelegate, FSteamID, SteamID, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnFriendsEnumerateFollowingListDelegate, ESteamResult, Result, const TArray<FSteamID>&, SteamIDs, int32, ResultsReturned, int32, TotalResults);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFriendsGetFollowerCountDelegate, ESteamResult, Result, FSteamID, SteamID, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFriendsIsFollowingDelegate, ESteamResult, Result, FSteamID, SteamID, bool, bIsFollowing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameConnectedChatJoinDelegate, FSteamID, SteamIDClanChat, FSteamID, SteamIDUser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameConnectedChatLeaveDelegate, FSteamID, SteamIDClanChat, FSteamID, SteamIDUser, bool, bKicked, bool, bDropped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameConnectedClanChatMsgDelegate, FSteamID, SteamIDClanChat, FSteamID, SteamIDUser, int32, MessageID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameConnectedFriendChatMsgDelegate, FSteamID, SteamIDUser, int32, MessageID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameLobbyJoinRequestedDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDFriend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOverlayActivatedDelegate, bool, bActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameRichPresenceJoinRequestedDelegate, FSteamID, SteamIDFriend, FString, ConnectionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameServerChangeRequestedDelegate, FString, IP, FString, Password);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJoinClanChatRoomCompletionResultDelegate, FSteamID, SteamIDClanChat, ESteamChatRoomEnterResponse, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPersonaStateChangeDelegate, FSteamID, SteamID, ESteamPersonaChange, PersonaStateChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetPersonaNameResponseDelegate, bool, bSuccess, bool, bLocalSuccess, ESteamResult, Result);

class UTexture2D;

/**
 * Functions for accessing and manipulating Steam friends information.
 * https://partner.steamgames.com/doc/api/ISteamFriends
 */
UCLASS()
class STEAMBRIDGE_API USteamFriends final : public UObject
{
	GENERATED_BODY()

public:
	USteamFriends();
	~USteamFriends();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Friends", CompactNodeTitle = "SteamFriends"))
	static USteamFriends* GetSteamFriends() { return USteamFriends::StaticClass()->GetDefaultObject<USteamFriends>(); }

	/**
	 * Activates the Steam Overlay to a specific dialog.
	 * This is equivalent to calling ActivateGameOverlayToUser with steamID set to ISteamUser::GetSteamID.
	 *
	 * @param const ESteamOverlayTypes OverlayType - The dialog to open.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlay(const ESteamGameOverlayTypes OverlayType);

	/**
	 * Activates the Steam Overlay to open the invite dialog. Invitations sent from this dialog will be for the provided lobby.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby that selected users will be invited to.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayInviteDialog(FSteamID SteamIDLobby) { SteamFriends()->ActivateGameOverlayInviteDialog(SteamIDLobby); }

	/**
	 * Activates the Steam Overlay to the Steam store page for the provided app.
	 * Using k_uAppIdInvalid brings the user to the front page of the Steam store.
	 *
	 * @param int32 AppID - The app ID to show the store page of.
	 * @param ESteamOverlayToStoreFlag StoreFlag - 	Flags to modify the behavior when the page opens.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToStore(int32 AppID, ESteamOverlayToStoreFlag StoreFlag) { SteamFriends()->ActivateGameOverlayToStore(AppID, (EOverlayToStoreFlag)StoreFlag); }

	/**
	 * Activates Steam Overlay to a specific dialog.
	 *
	 * @param const ESteamGameUserOverlayTypes OverlayType - The dialog to open.
	 * @param FSteamID SteamID - The Steam ID of the context to open this dialog to.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToUser(const ESteamGameUserOverlayTypes OverlayType, FSteamID SteamID);

	/**
	 * Activates Steam Overlay web browser directly to the specified URL.
	 *
	 * @param const FString & URL - The webpage to open. (A fully qualified address with the protocol is required, e.g. "http://www.steampowered.com")
	 * @param bool bShowModal
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToWebPage(const FString& URL, bool bShowModal = false) { SteamFriends()->ActivateGameOverlayToWebPage(TCHAR_TO_UTF8(*URL), bShowModal ? k_EActivateGameOverlayToWebPageMode_Modal : k_EActivateGameOverlayToWebPageMode_Default); }

	/**
	 * Clears all of the current user's Rich Presence key/values.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ClearRichPresence() { SteamFriends()->ClearRichPresence(); }

	/**
	 * Closes the specified Steam group chat room in the Steam UI.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the Steam group chat room to close.
	 * @return bool - true if the user successfully left the Steam group chat room. false if the user is not in the provided Steam group chat room.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool CloseClanChatWindowInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->CloseClanChatWindowInSteam(SteamIDClanChat); }

	/**
	 * Refresh the Steam Group activity data or get the data from groups other than one that the current user is a member.
	 * After receiving the callback you can then use GetClanActivityCounts to get the up to date user counts.
	 *
	 * @param TArray<FSteamID> & SteamClanIDs - A list of steam groups to get the updated data for.
	 * @param int32 ClansToRequest - This MUST be the number of groups in SteamClanIDs.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a DownloadClanActivityCountsResult_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall DownloadClanActivityCounts(TArray<FSteamID>& SteamClanIDs, int32 ClansToRequest = 1) const;

	/*UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Friends")
	TArray<FSteamID> EnumerateFollowingList() const;*/

	/**
	 * Gets the Steam ID at the given index in a Steam group chat.
	 * You must call GetClanChatMemberCount before calling this.
	 *
	 * @param FSteamID SteamIDClan - This MUST be the same source used in the previous call to GetClanChatMemberCount!
	 * @param int32 UserIndex - An index between 0 and GetClanChatMemberCount.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetChatMemberByIndex(FSteamID SteamIDClan, int32 UserIndex) const { return SteamFriends()->GetChatMemberByIndex(SteamIDClan, UserIndex).ConvertToUint64(); }

	/**
	 * Gets the most recent information we have about what the users in a Steam Group are doing.
	 * This can only retrieve data that the local client knows about. To refresh the data or get data from a group other than one that the current user is a member of you must call DownloadClanActivityCounts.
	 *
	 * @param FSteamID SteamIDClan - The Steam group to get the activity of.
	 * @param int32 & Online - Returns the number of members that are online.
	 * @param int32 & InGame - Returns the number members that are in game (excluding those with their status set to offline).
	 * @param int32 & Chatting - 	Returns the number of members in the group chat room.
	 * @return bool - true if the data was successfully returned. false if the provided Steam ID is invalid or the local client does not have info about the Steam group and sets all the other parameters to 0.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool GetClanActivityCounts(FSteamID SteamIDClan, int32& Online, int32& InGame, int32& Chatting) { return SteamFriends()->GetClanActivityCounts(SteamIDClan, &Online, &InGame, &Chatting); }

	/**
	 * Gets the Steam group's Steam ID at the given index.
	 *
	 * @param int32 ClanIndex - An index between 0 and GetClanCount.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetClanByIndex(int32 ClanIndex) const { return SteamFriends()->GetClanByIndex(ClanIndex).ConvertToUint64(); }

	/**
	 * Get the number of users in a Steam group chat.
	 * Large steam groups cannot be iterated by the local user.
	 * The current user must be in a lobby to retrieve the Steam IDs of other users in that lobby.
	 * This is used for iteration, after calling this then GetChatMemberByIndex can be used to get the Steam ID of each person in the chat.
	 *
	 * @param FSteamID  SteamIDClan - The Steam group to get the chat count of.
	 * @return int32 - 0 if the Steam ID provided is invalid or if the local user doesn't have the data available.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanChatMemberCount(FSteamID SteamIDClan) const { return SteamFriends()->GetClanChatMemberCount(SteamIDClan); }

	/**
	 * Gets the data from a Steam group chat room message.
	 * This should only ever be called in response to a GameConnectedClanChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the Steam group chat room.
	 * @param int32 MessageID - The index of the message. This should be the m_iMessageID field of GameConnectedClanChatMsg_t.
	 * @param FString & Message - The buffer where the chat message will be copied into. (Should be big enough to hold 2048 UTF-8 characters. So 8192 bytes + 1 for '\0')
	 * @param ESteamChatEntryType & ChatEntryType - Returns the type of chat entry that was received.
	 * @param FSteamID & SteamIDChatter - Returns the Steam ID of the user that sent the message.
	 * @return int32 - The number of bytes copied into prgchText. Returns 0 and sets peChatEntryType to k_EChatEntryTypeInvalid if the current user is not in the specified Steam group chat room or if the index provided in iMessage is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanChatMessage(FSteamID SteamIDClanChat, int32 MessageID, FString& Message, ESteamChatEntryType& ChatEntryType, FSteamID& SteamIDChatter);

	/**
	 * Gets the number of Steam groups that the current user is a member of.
	 * This is used for iteration, after calling this then GetClanByIndex can be used to get the Steam ID of each Steam group.
	 *
	 * @return int32 - The number of Steam groups that the user is a member of.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanCount() const { return SteamFriends()->GetClanCount(); }

	/**
	 * Gets the display name for the specified Steam group; if the local client knows about it.
	 *
	 * @param FSteamID SteamIDClan - The Steam group to get the name of.
	 * @return FString - The Steam groups name in UTF-8 format. Returns an empty string ("") if the provided Steam ID is invalid or the user does not know about the group.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetClanName(FSteamID SteamIDClan) const { return SteamFriends()->GetClanName(SteamIDClan); }

	/**
	 * Gets the Steam ID of the officer at the given index in a Steam group.
	 * You must call GetClanOfficerCount before calling this.
	 *
	 * @param FSteamID SteamIDClan - This must be the same steam group used in the previous call to GetClanOfficerCount!
	 * @param int32 OfficerIndex - An index between 0 and GetClanOfficerCount.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetClanOfficerByIndex(FSteamID SteamIDClan, int32 OfficerIndex) const { return SteamFriends()->GetClanOfficerByIndex(SteamIDClan, OfficerIndex).ConvertToUint64(); }

	/**
	 * Gets the number of officers (administrators and moderators) in a specified Steam group.
	 * This also includes the owner of the Steam group.
	 * This is used for iteration, after calling this then GetClanOfficerByIndex can be used to get the Steam ID of each officer.
	 * You must call RequestClanOfficerList before this to get the required data!
	 *
	 * @param FSteamID SteamIDClan - The Steam group to get the officer count of.
	 * @return int32 - The number of officers in the Steam group. Returns 0 if steamIDClan is invalid or if RequestClanOfficerList has not been called for it.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanOfficerCount(FSteamID SteamIDClan) const { return SteamFriends()->GetClanOfficerCount(SteamIDClan); }

	/**
	 * Gets the owner of a Steam Group.
	 * You must call RequestClanOfficerList before this to get the required data!
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the Steam group to get the owner for.
	 * @return FSteamID - Returns k_steamIDNil if SteamIDClan is invalid or if RequestClanOfficerList has not been called for it.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetClanOwner(FSteamID SteamIDClan) const { return SteamFriends()->GetClanOwner(SteamIDClan).ConvertToUint64(); }

	/**
	 * Gets the unique tag (abbreviation) for the specified Steam group; If the local client knows about it.
	 * The Steam group abbreviation is a unique way for people to identify the group and is limited to 12 characters. In some games this will appear next to the name of group members.
	 *
	 * @param FSteamID SteamIDClan - The Steam group to get the tag of.
	 * @return FString - The Steam groups tag in UTF-8 format. Returns an empty string ("") if the provided Steam ID is invalid or the user does not know about the group.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetClanTag(FSteamID SteamIDClan) const { return SteamFriends()->GetClanTag(SteamIDClan); }

	/**
	 * Gets the Steam ID of the recently played with user at the given index.
	 * You must call GetCoplayFriendCount before calling this.
	 *
	 * @param int32 CoplayFriendIndex - An index between 0 and GetCoplayFriendCount.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetCoplayFriend(int32 CoplayFriendIndex) const { return SteamFriends()->GetCoplayFriend(CoplayFriendIndex).ConvertToUint64(); }

	/**
	 * Gets the number of players that the current users has recently played with, across all games.
	 * This is used for iteration, after calling this then GetCoplayFriend can be used to get the Steam ID of each player.
	 * These players are have been set with previous calls to SetPlayedWith.
	 *
	 * @return int32 - The number of users that the current user has recently played with.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetCoplayFriendCount() const { return SteamFriends()->GetCoplayFriendCount(); }

	/**
	 * Gets the number of users following the specified user.
	 *
	 * @param FSteamID SteamID - The user to get the follower count for.
	 * @return int32 - SteamAPICall_t to be used with a FriendsGetFollowerCount_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFollowerCount(FSteamID SteamID) const { return SteamFriends()->GetFollowerCount(SteamID); }

	/**
	 * Gets the Steam ID of the user at the given index.
	 * You must call GetFriendCount before calling this.
	 *
	 * @param int32 FriendIndex - An index between 0 and GetFriendCount.
	 * @param const TArray<ESteamFriendFlags> & FriendFlags - A combined union (binary "or") of EFriendFlags. This must be the same value as used in the previous call to GetFriendCount.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetFriendByIndex(int32 FriendIndex, const TArray<ESteamFriendFlags>& FriendFlags) const;

	/**
	 * Gets the app ID of the game that user played with someone on their recently-played-with list.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the user on the recently-played-with list to get the game played.
	 * @return int32 - Steam IDs not in the recently-played-with list return k_uAppIdInvalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCoplayGame(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendCoplayGame(SteamIDFriend); }

	/**
	 * Gets the timestamp of when the user played with someone on their recently-played-with list.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the user on the recently-played-with list to get the timestamp for.
	 * @return int32 - The time is provided in Unix epoch format (seconds since Jan 1st 1970). Steam IDs not in the recently-played-with list return 0.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCoplayTime(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendCoplayTime(SteamIDFriend); }

	/**
	 * NOTE: This method has been rewritten to use FDateTime instead of epoch time. GetFriendCoplayTime is the original method.
	 * Gets the timestamp of when the user played with someone on their recently-played-with list.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the user on the recently-played-with list to get the timestamp for.
	 * @return FDateTime - The time is provided in a friendly format. Steam IDs not in the recently-played-with list return 0.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FDateTime GetFriendCoplayDateTime(FSteamID SteamIDFriend) const { return FDateTime::FromUnixTimestamp(SteamFriends()->GetFriendCoplayTime(SteamIDFriend)); }

	/**
	 * Gets the number of users the client knows about who meet a specified criteria. (Friends, blocked, users on the same server, etc)
	 * This can be used to iterate over all of the users by calling GetFriendByIndex to get the Steam IDs of each user.
	 *
	 * @param const TArray<ESteamFriendFlags> & FriendFlags - A combined union (binary "or") of one or more EFriendFlags.
	 * @return int32 - The number of users that meet the specified criteria.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCount(const TArray<ESteamFriendFlags>& FriendFlags) const;

	/**
	 * Get the number of users in a source (Steam group, chat room, lobby, or game server).
	 * Large Steam groups cannot be iterated by the local user.
	 * If you're getting the number of lobby members then you should use ISteamMatchmaking::GetNumLobbyMembers instead.
	 * This is used for iteration, after calling this then GetFriendFromSourceByIndex can be used to get the Steam ID of each person in the source.
	 *
	 * @param const FSteamID & SteamIDSource - The Steam group, chat room, lobby or game server to get the user count of.
	 * @return int32 - 0 if the Steam ID provided is invalid or if the local user doesn't have the data available.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCountFromSource(FSteamID SteamIDSource) const { return SteamFriends()->GetFriendCountFromSource(SteamIDSource); }

	/**
	 * Gets the Steam ID at the given index from a source (Steam group, chat room, lobby, or game server).
	 * You must call GetFriendCountFromSource before calling this.
	 *
	 * @param FSteamID SteamIDSource - 	This MUST be the same source used in the previous call to GetFriendCountFromSource!
	 * @param int32 FriendIndex - An index between 0 and GetFriendCountFromSource.
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetFriendFromSourceByIndex(FSteamID SteamIDSource, int32 FriendIndex) const { return SteamFriends()->GetFriendFromSourceByIndex(SteamIDSource, FriendIndex).ConvertToUint64(); }

	/**
	 * Checks if the specified friend is in a game, and gets info about the game if they are.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the other user.
	 * @param FSteamID & GameID - The game ID that the friend is playing.
	 * @param FString & GameIP - The IP of the server the friend is playing on.
	 * @param int32 & GamePort - The port of the server the friend is playing on.
	 * @param int32 & QueryPort - The query port of the server the friend is playing on.
	 * @param FSteamID & SteamIDLobby - The Steam ID of the lobby the friend is in
	 * @return bool - true if the user is a friend and is in a game; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool GetFriendGamePlayed(FSteamID SteamIDFriend, FSteamID& GameID, FString& GameIP, int32& GamePort, int32& QueryPort, FSteamID& SteamIDLobby);

	/**
	 * Gets the data from a Steam friends message.
	 * This should only ever be called in response to a GameConnectedFriendChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the friend that sent this message.
	 * @param int32 MessageIndex - The index of the message. This should be the m_iMessageID field of GameConnectedFriendChatMsg_t.
	 * @param FString & Message - The buffer where the chat message will be copied into.
	 * @param ESteamChatEntryType & ChatEntryType - Returns the type of chat entry that was received.
	 * @return int32 - The number of bytes copied into pvData. Returns 0 and sets peChatEntryType to k_EChatEntryTypeInvalid if the current user is chat restricted, if the provided Steam ID is not a friend, or if the index provided in iMessageID is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendMessage(FSteamID SteamIDFriend, int32 MessageIndex, FString& Message, ESteamChatEntryType& ChatEntryType);

	/**
	 * Gets the specified user's persona (display) name.
	 * This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user.
	 * Upon on first joining a lobby, chat room, or game server the current user will not known the name of the other users automatically; that information will arrive asynchronously via PersonaStateChange_t callbacks.
	 * To get the persona name of the current user use GetPersonaName.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the other user.
	 * @return FString - The current users persona name in UTF-8 format. Guaranteed to not be NULL. Returns an empty string (""), or "[unknown]" if the Steam ID is invalid or not known to the caller.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendPersonaName(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendPersonaName(SteamIDFriend); }

	/**
	 * Gets one of the previous display names for the specified user.
	 * This only works for display names that the current user has seen on the local computer.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the other user.
	 * @param int32 NameHistoryIndex - The index of the history to receive. 0 is their current persona name, 1 is their most recent before they changed it, etc.
	 * @return FString - The players old persona name at the given index. Returns an empty string when there are no further items in the history.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendPersonaNameHistory(FSteamID SteamIDFriend, int32 NameHistoryIndex = 0) const { return SteamFriends()->GetFriendPersonaNameHistory(SteamIDFriend, NameHistoryIndex); }

	/**
	 * Gets the current status of the specified user.
	 * This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user.
	 * To get the state of the current user use GetPersonaState.
	 *
	 * @param FString SteamIDFriend - The Steam ID of the other user.
	 * @return ESteamPersonaState - The friend state of the specified user. (Online, Offline, In-Game, etc)
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamPersonaState GetFriendPersonaState(FSteamID SteamIDFriend) const { return (ESteamPersonaState)SteamFriends()->GetFriendPersonaState(SteamIDFriend); }

	/**
	 * Gets a relationship to a specified user.
	 *
	 * @param FSteamID SteamIDFriend - 	The Steam ID of the other user.
	 * @return ESteamFriendRelationship - How the users know each other.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamFriendRelationship GetFriendRelationship(FSteamID SteamIDFriend) const { return (ESteamFriendRelationship)SteamFriends()->GetFriendRelationship(SteamIDFriend); }

	/**
	 * Get a Rich Presence value from a specified friend.
	 *
	 * @param FSteamID SteamIDFriend - The friend to get the Rich Presence value for.
	 * @param const FString & Key - The Rich Presence key to request.
	 * @return FString - Returns an empty string ("") if the specified key is not set.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendRichPresence(FSteamID SteamIDFriend, const FString& Key) const { return SteamFriends()->GetFriendRichPresence(SteamIDFriend, TCHAR_TO_UTF8(*Key)); }

	/**
	 * Get a Rich Presence value from a specified friend by index.
	 *
	 * @param FSteamID SteamIDFriend - This should be the same user provided to the previous call to GetFriendRichPresenceKeyCount!
	 * @param int32 KeyIndex - An index between 0 and GetFriendRichPresenceKeyCount.
	 * @return FString - Returns an empty string ("") if the index is invalid or the specified user has no Rich Presence data available.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendRichPresenceKeyByIndex(FSteamID SteamIDFriend, int32 KeyIndex) const { return SteamFriends()->GetFriendRichPresenceKeyByIndex(SteamIDFriend, KeyIndex); }

	/**
	 * Gets the number of Rich Presence keys that are set on the specified user.
	 * This is used for iteration, after calling this then GetFriendRichPresenceKeyByIndex to get the rich presence keys.
	 * This is typically only ever used for debugging purposes.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the user to get the Rich Presence Key Count of.
	 * @return int32 - Returns 0 if there is no Rich Presence information for the specified user.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendRichPresenceKeyCount(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendRichPresenceKeyCount(SteamIDFriend); }

	/**
	 * Gets the number of friends groups (tags) the user has created.
	 * This is used for iteration, after calling this then GetFriendsGroupIDByIndex can be used to get the ID of each friend group.
	 * This is not to be confused with Steam groups. Those can be obtained with GetClanCount.
	 *
	 * @return int32 - The number of friends groups the current user has.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendsGroupCount() const { return SteamFriends()->GetFriendsGroupCount(); }

	/**
	 * Gets the friends group ID for the given index.
	 * You must call GetFriendsGroupCount before calling this.
	 *
	 * @param int32 GroupIndex - An index between 0 and GetFriendsGroupCount.
	 * @return FSteamFriendsGroupID - Invalid indices return k_FriendsGroupID_Invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamFriendsGroupID GetFriendsGroupIDByIndex(int32 GroupIndex = 0) const { return SteamFriends()->GetFriendsGroupIDByIndex(GroupIndex); }

	/**
	 * Gets the number of friends in a given friends group.
	 * This should be called before getting the list of friends with GetFriendsGroupMembersList.
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID - The friends group ID to get the number of friends in.
	 * @return int32 - The number of friends in the specified friends group.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendsGroupMembersCount(FSteamFriendsGroupID FriendsGroupID) const { return SteamFriends()->GetFriendsGroupMembersCount(FriendsGroupID); }

	/**
	 * Gets the number of friends in the given friends group.
	 * If fewer friends exist than requested those positions' Steam IDs will be invalid.
	 * You must call GetFriendsGroupMembersCount before calling this to set up the pOutSteamIDMembers array with an appropriate size!
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID - The friends group ID to get the members list of.
	 * @param TArray<FSteamID> & MemberSteamIDs - Returns the Steam IDs of the friends by setting them in this array.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void GetFriendsGroupMembersList(FSteamFriendsGroupID FriendsGroupID, TArray<FSteamID>& MemberSteamIDs);

	/**
	 * Gets the name for the given friends group.
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID - The friends group ID to get the name of.
	 * @return FString - The friend groups name in UTF-8 format. Returns NULL if the group ID is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendsGroupName(FSteamFriendsGroupID FriendsGroupID) const { return SteamFriends()->GetFriendsGroupName(FriendsGroupID); }

	/**
	 * Gets the Steam level of the specified user.
	 * You can use the local users Steam ID (ISteamUser::GetSteamID) to get their level.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the user.
	 * @return int32 - The Steam level if it's available. If the Steam level is not immediately available for the specified user then this returns 0 and queues it to be downloaded from the Steam servers.
	 * When it gets downloaded a PersonaStateChange_t callback will be posted with m_nChangeFlags including k_EPersonaChangeSteamLevel.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendSteamLevel(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendSteamLevel(SteamIDFriend); }

	/**
	 * Gets a handle to the avatar for the specified user.
	 * You can pass in ISteamUser::GetSteamID to get the current users avatar.
	 * This only works for users that the local user knows about. They will automatically know about their friends, people on leaderboards they've requested, or people in the same source as them -
	 * (Steam group, chat room, lobby, or game server). If they don't know about them then you must call RequestUserInformation to cache the avatar locally.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param ESteamAvatarSize AvatarSize
	 * @param int32& ImageHandle
	 * @return UTexture2D*
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	UTexture2D* GetFriendAvatar(FSteamID SteamIDFriend, ESteamAvatarSize AvatarSize, int32& ImageHandle) const;

	/**
	 * Gets the current users persona (display) name.
	 * This is the same name that is displayed the users community profile page.
	 * To get the persona name of other users use GetFriendPersonaName.
	 *
	 * @return FString - The current users persona name in UTF-8 format. Guaranteed to not be NULL.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetPersonaName() const { return SteamFriends()->GetPersonaName(); }

	/**
	 * Gets the friend status of the current user.
	 * To get the state of other users use GetFriendPersonaState.
	 *
	 * @return ESteamPersonaState - The friend state of the current user. (Online, Offline, In-Game, etc)
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamPersonaState GetPersonaState() const { return (ESteamPersonaState)SteamFriends()->GetPersonaState(); }

	/**
	 * Gets the nickname that the current user has set for the specified user.
	 *
	 * @param FSteamID SteamIDPlayer - The Steam ID of the user.
	 * @return FString - NULL if the no nickname has been set for that user.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetPlayerNickname(FSteamID SteamIDPlayer) const { return SteamFriends()->GetPlayerNickname(SteamIDPlayer); }

	/**
	 * Checks if the user meets the specified criteria. (Friends, blocked, users on the same server, etc)
	 *
	 * @return TArray<ESteamUserRestrictions>
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	TArray<ESteamUserRestrictions> GetUserRestrictions() const;

	/**
	 * Checks if the user meets the specified criteria. (Friends, blocked, users on the same server, etc)
	 *
	 * @param FSteamID SteamIDFriend - The Steam user to check the friend status of.
	 * @param const TArray<ESteamFriendFlags> & FriendFlags - A combined union (binary "or") of one or more EFriendFlags.
	 * @return bool - true if the specified user meets any of the criteria specified in iFriendFlags; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool HasFriend(FSteamID SteamIDFriend, const TArray<ESteamFriendFlags>& FriendFlags);

	/**
	 * Invites a friend or clan member to the current game using a special invite string.
	 * If the target user accepts the invite then the pchConnectString gets added to the command-line when launching the game.
	 * If the game is already running for that user, then they will receive a GameRichPresenceJoinRequested_t callback with the connect string.
	 * Triggers a GameRichPresenceJoinRequested_t callback.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the friend to invite.
	 * @param const FString & ConnectString - A string that lets the friend know how to join the game (I.E. the game server IP). This can not be longer than specified in k_cchMaxRichPresenceValueLength.
	 * @return bool - true if the invite was successfully sent. false under the following conditions:
	 * The Steam ID provided to steamIDFriend was invalid.
	 * The Steam ID provided to steamIDFriend is not a friend or does not share the same Steam Group as the current user.
	 * The value provided to pchConnectString was too long.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool InviteUserToGame(FSteamID SteamIDFriend, const FString& ConnectString) const { return SteamFriends()->InviteUserToGame(SteamIDFriend, TCHAR_TO_UTF8(*ConnectString)); }

	/**
	 * Checks if a user in the Steam group chat room is an admin.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the Steam group chat room.
	 * @param FSteamID SteamIDUser - The Steam ID of the user to check the admin status of.
	 * @return bool - true if the specified user is an admin. false if the user is not an admin, if the current user is not in the chat room specified, or the specified user is not in the chat room.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanChatAdmin(FSteamID SteamIDClanChat, FSteamID SteamIDUser) const { return SteamFriends()->IsClanChatAdmin(SteamIDClanChat, SteamIDUser); }

	/**
	 * Checks if the Steam group is public.
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the Steam group.
	 * @return bool - true if the specified group is public, false if the specified group is not public
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanPublic(FSteamID SteamIDClan) const { return SteamFriends()->IsClanPublic(SteamIDClan); }

	/**
	 * Checks if the Steam group is an official game group/community hub.
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the Steam group.
	 * @return bool - true if the specified group is an official game group/community hub, false if the specified group is not an official game group/community hub
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanOfficialGameGroup(FSteamID SteamIDClan) const { return SteamFriends()->IsClanOfficialGameGroup(SteamIDClan); }

	/**
	 * Checks if the Steam Group chat room is open in the Steam UI.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the Steam group chat room to check.
	 * @return bool - true if the specified Steam group chat room is opened; otherwise, false. This also returns false if the specified Steam group chat room is unknown.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanChatWindowOpenInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->IsClanChatWindowOpenInSteam(SteamIDClanChat); }

	/**
	 * Checks if the current user is following the specified user.
	 *
	 * @param FSteamID SteamID - The Steam ID of the check if we are following.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a FriendsIsFollowing_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall IsFollowing(FSteamID SteamID) const { return SteamFriends()->IsFollowing(SteamID); }

	/**
	 * Checks if a specified user is in a source (Steam group, chat room, lobby, or game server).
	 *
	 * @param FSteamID SteamIDUser - The user to check if they are in the source.
	 * @param FSteamID SteamIDSource - 	The source to check for the user.
	 * @return bool - true if the local user can see that steamIDUser is a member or in steamIDSource; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsUserInSource(FSteamID SteamIDUser, FSteamID SteamIDSource) const { return SteamFriends()->IsUserInSource(SteamIDUser, SteamIDSource); }

	/**
	 * Allows the user to join Steam group (clan) chats right within the game.
	 * The behavior is somewhat complicated, because the user may or may not be already in the group chat from outside the game or in the overlay.
	 * You can use ActivateGameOverlayToUser to open the in-game overlay version of the chat.
	 * If you have joined a Steam group chat then you should be watching for the following callbacks:
	 *  GameConnectedClanChatMsg_t
	 *  GameConnectedChatJoin_t
	 *  GameConnectedChatLeave_t
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the Steam group to join.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a JoinClanChatRoomCompletionResult_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall JoinClanChatRoom(FSteamID SteamIDClan) const { return SteamFriends()->JoinClanChatRoom(SteamIDClan); }

	/**
	 * Leaves a Steam group chat that the user has previously entered with JoinClanChatRoom.
	 * Triggers a GameConnectedChatLeave_t callback.
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the Steam group chat to leave.
	 * @return bool - true if user is in the specified chat room, otherwise false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool LeaveClanChatRoom(FSteamID SteamIDClan) const { return SteamFriends()->LeaveClanChatRoom(SteamIDClan); }

	/**
	 * Opens the specified Steam group chat room in the Steam UI.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the Steam group chat room to open.
	 * @return bool - true if the user successfully entered the Steam group chat room. false in one of the following situations:
	 * The provided Steam group chat room does not exist or the user does not have access to join it.
	 * The current user is currently rate limited.
	 * The current user is chat restricted.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool OpenClanChatWindowInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->OpenClanChatWindowInSteam(SteamIDClanChat); }

	/**
	 * Sends a message to a Steam friend.
	 *
	 * @param FSteamID SteamIDFriend - The Steam ID of the friend to send the message to.
	 * @param const FString & MsgToSend - The UTF-8 formatted message to send.
	 * @return bool - true if the message was successfully sent. false if the current user is rate limited or chat restricte
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool ReplyToFriendMessage(FSteamID SteamIDFriend, const FString& MsgToSend) const { return SteamFriends()->ReplyToFriendMessage(SteamIDFriend, TCHAR_TO_UTF8(*MsgToSend)); }

	/**
	 * Requests information about a Steam group officers (administrators and moderators).
	 * You can only ask about Steam groups that a user is a member of.
	 * This won't download avatars for the officers automatically. If no avatar image is available for an officer, then call RequestUserInformation to download the avatar.
	 *
	 * @param FSteamID SteamIDClan - The Steam group to get the officers list for.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a ClanOfficerListResponse_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall RequestClanOfficerList(FSteamID SteamIDClan) const { return SteamFriends()->RequestClanOfficerList(SteamIDClan); }

	/**
	 * Requests Rich Presence data from a specific user.
	 * This is used to get the Rich Presence information from a user that is not a friend of the current user, like someone in the same lobby or game server.
	 * This function is rate limited, if you call this too frequently for a particular user then it will just immediately post a callback without requesting new data from the server.
	 *
	 * @param FSteamID SteamIDFriend - 	The Steam ID of the user to request the rich presence of.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void RequestFriendRichPresence(FSteamID SteamIDFriend) { SteamFriends()->RequestFriendRichPresence(SteamIDFriend); }

	/**
	 * Requests the persona name and optionally the avatar of a specified user.
	 * It's a lot slower to download avatars and churns the local cache, so if you don't need avatars, don't request them.
	 * Triggers a PersonaStateChange_t callback.
	 *
	 * @param FSteamID SteamIDUser - The user to request the information of.
	 * @param bool bRequireNameOnly - Retrieve the Persona name only (true)? Or both the name and the avatar (false)?
	 * @return bool - true means that the data has being requested, and a PersonaStateChange_t callback will be posted when it's retrieved.
	 * false means that we already have all the details about that user, and functions that require this information can be used immediately.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool RequestUserInformation(FSteamID SteamIDUser, bool bRequireNameOnly) const { return SteamFriends()->RequestUserInformation(SteamIDUser, bRequireNameOnly); }

	/**
	 * Sends a message to a Steam group chat room.
	 *
	 * @param FSteamID SteamIDClanChat - The Steam ID of the group chat to send the message to.
	 * @param const FString & Text - The UTF-8 formatted message to send. This can be up to 2048 characters long.
	 * @return bool - true if the message was successfully sent. false under one of the following circumstances:
	 * The current user is not in the specified group chat.
	 * The current user is not connected to Steam.
	 * The current user is rate limited.
	 * The current user is chat restricted.
	 * The message in pchText exceeds 2048 characters.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SendClanChatMessage(FSteamID SteamIDClanChat, const FString& Text) const { return SteamFriends()->SendClanChatMessage(SteamIDClanChat, TCHAR_TO_UTF8(*Text)); }

	/**
	 * Let Steam know that the user is currently using voice chat in game.
	 * This will suppress the microphone for all voice communication in the Steam UI.
	 *
	 * @param FSteamID SteamIDUser - Unused
	 * @param bool bSpeaking - Did the user start speaking in game (true) or stopped speaking in game (false)?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void SetInGameVoiceSpeaking(FSteamID SteamIDUser, bool bSpeaking) { SteamFriends()->SetInGameVoiceSpeaking(SteamIDUser, bSpeaking); }

	/**
	 * Listens for Steam friends chat messages.
	 * You can then show these chats inline in the game. For example with a Blizzard style chat message system or the chat system in Dota 2.
	 * After enabling this you will receive GameConnectedFriendChatMsg_t callbacks when ever the user receives a chat message. You can get the actual message data from this callback with GetFriendMessage. You can send messages with ReplyToFriendMessage.
	 * Triggers a GameConnectedFriendChatMsg_t callback.
	 *
	 * @param bool bInterceptEnabled - 	Turn friends message interception on (true) or off (false)?
	 * @return bool - Always returns true
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SetListenForFriendsMessages(bool bInterceptEnabled) const { return SteamFriends()->SetListenForFriendsMessages(bInterceptEnabled); }

	/**
	 * Sets the current users persona name, stores it on the server and publishes the changes to all friends who are online.
	 * Changes take place locally immediately, and a PersonaStateChange_t callback is posted, presuming success.
	 * If the name change fails to happen on the server, then an additional PersonaStateChange_t callback will be posted to change the name back, in addition to the final result available in the call result.
	 *
	 * @param const FString & PersonaName - The users new persona name. Can not be longer than k_cchPersonaNameMax bytes.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a SetPersonaNameResponse_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall SetPersonaName(const FString& PersonaName) const { return SteamFriends()->SetPersonaName(TCHAR_TO_UTF8(*PersonaName)); }

	/**
	 * Mark a target user as 'played with'.
	 * The current user must be in game with the other player for the association to work.
	 *
	 * @param FSteamID SteamIDUserPlayedWith - The other user that we have played with.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void SetPlayedWith(FSteamID SteamIDUserPlayedWith) { SteamFriends()->SetPlayedWith(SteamIDUserPlayedWith); }

	/**
	 * Sets a Rich Presence key/value for the current user that is automatically shared to all friends playing the same game.
	 *
	 * @param const FString & Key - The rich presence 'key' to set. This can not be longer than specified in k_cchMaxRichPresenceKeyLength.
	 * @param const FString & Value - The rich presence 'value' to associate with pchKey. This can not be longer than specified in k_cchMaxRichPresenceValueLength.
	 * If this is set to an empty string ("") or NULL then the key is removed if it's set.
	 * @return bool - true if the rich presence was set successfully. false under the following conditions:
	 * pchKey was longer than k_cchMaxRichPresenceKeyLength or had a length of 0.
	 * pchValue was longer than k_cchMaxRichPresenceValueLength.
	 * The user has reached the maximum amount of rich presence keys as defined by k_cchMaxRichPresenceKeys.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SetRichPresence(const FString& Key, const FString& Value) const { return SteamFriends()->SetRichPresence(TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/** Delegates */

	/** Called when a large avatar is loaded if you have tried requesting it when it was unavailable. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnAvatarImageLoaded"))
	FOnAvatarImageLoadedDelegate m_OnAvatarImageLoaded;

	/** Marks the return of a request officer list call. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnClanOfficerListResponse"))
	FOnClanOfficerListResponseDelegate m_OnClanOfficerListResponse;

	/** Called when a Steam group activity has received. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnDownloadClanActivityCountsResult"))
	FOnDownloadClanActivityCountsResultDelegate m_OnDownloadClanActivityCountsResult;

	/** Called when Rich Presence data has been updated for a user, this can happen automatically when friends in the same game update their rich presence, or after a call to RequestFriendRichPresence. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendRichPresenceUpdate"))
	FOnFriendRichPresenceUpdateDelegate m_OnFriendRichPresenceUpdate;

	/** Returns the result of EnumerateFollowingList. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsEnumerateFollowingList"))
	FOnFriendsEnumerateFollowingListDelegate m_OnFriendsEnumerateFollowingList;

	/** Returns the result of GetFollowerCount. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsGetFollowerCount"))
	FOnFriendsGetFollowerCountDelegate m_OnFriendsGetFollowerCount;

	/** Returns the result of IsFollowing. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsIsFollowing"))
	FOnFriendsIsFollowingDelegate m_OnFriendsIsFollowing;

	/** Called when a user has joined a Steam group chat that the we are in. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedChatJoin"))
	FOnGameConnectedChatJoinDelegate m_OnGameConnectedChatJoin;

	/** Called when a user has left a Steam group chat that the we are in. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedChatLeave"))
	FOnGameConnectedChatLeaveDelegate m_OnGameConnectedChatLeave;

	/** Called when a chat message has been received in a Steam group chat that we are in. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedClanChatMsg"))
	FOnGameConnectedClanChatMsgDelegate m_OnGameConnectedClanChatMsg;

	/** Called when chat message has been received from a friend. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedFriendChatMsg"))
	FOnGameConnectedFriendChatMsgDelegate m_OnGameConnectedFriendChatMsg;

	/**
	 * Called when the user tries to join a lobby from their friends list or from an invite. The game client should attempt to connect to specified lobby when this is received.
	 * If the game isn't running yet then the game will be automatically launched with the command line parameter +connect_lobby <64-bit lobby Steam ID> instead.
	 * NOTE: This callback is made when joining a lobby. If the user is attempting to join a game but not a lobby, then the callback GameRichPresenceJoinRequested_t will be made.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameLobbyJoinRequested"))
	FOnGameLobbyJoinRequestedDelegate m_OnGameLobbyJoinRequested;

	/** Posted when the Steam Overlay activates or deactivates. The game can use this to be pause or resume single player games. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameOverlayActivated"))
	FOnGameOverlayActivatedDelegate m_OnGameOverlayActivated;

	/**
	 * Called when the user tries to join a game from their friends list or after a user accepts an invite by a friend with InviteUserToGame.
	 * NOTE: This callback is made when joining a game. If the user is attempting to join a lobby, then the callback GameLobbyJoinRequested_t will be made.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameRichPresenceJoinRequested"))
	FOnGameRichPresenceJoinRequestedDelegate m_OnGameRichPresenceJoinRequested;

	/** Called when the user tries to join a different game server from their friends list. The game client should attempt to connect to specified server when this is received. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameServerChangeRequested"))
	FOnGameServerChangeRequestedDelegate m_OnGameServerChangeRequested;

	/** Posted when the user has attempted to join a Steam group chat via JoinClanChatRoom */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnJoinClanChatRoomCompletionResult"))
	FOnJoinClanChatRoomCompletionResultDelegate m_OnJoinClanChatRoomCompletionResult;

	/** Called whenever a friends' status changes. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnPersonaStateChange"))
	FOnPersonaStateChangeDelegate m_OnPersonaStateChange;

	/** Reports the result of an attempt to change the current user's persona name. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnSetPersonaNameResponse"))
	FOnSetPersonaNameResponseDelegate m_OnSetPersonaNameResponse;

private:
	STEAM_CALLBACK_MANUAL(USteamFriends, OnAvatarImageLoaded, AvatarImageLoaded_t, OnAvatarImageLoadedCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnClanOfficerListResponse, ClanOfficerListResponse_t, OnClanOfficerListResponseCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnDownloadClanActivityCountsResult, DownloadClanActivityCountsResult_t, OnDownloadClanActivityCountsResultCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnFriendRichPresenceUpdate, FriendRichPresenceUpdate_t, OnFriendRichPresenceUpdateCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnFriendsEnumerateFollowingList, FriendsEnumerateFollowingList_t, OnFriendsEnumerateFollowingListCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnFriendsGetFollowerCount, FriendsGetFollowerCount_t, OnFriendsGetFollowerCountCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnFriendsIsFollowing, FriendsIsFollowing_t, OnFriendsIsFollowingCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameConnectedChatJoin, GameConnectedChatJoin_t, OnGameConnectedChatJoinCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameConnectedChatLeave, GameConnectedChatLeave_t, OnGameConnectedChatLeaveCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameConnectedClanChatMsg, GameConnectedClanChatMsg_t, OnGameConnectedClanChatMsgCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameConnectedFriendChatMsg, GameConnectedFriendChatMsg_t, OnGameConnectedFriendChatMsgCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameLobbyJoinRequested, GameLobbyJoinRequested_t, OnGameLobbyJoinRequestedCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameOverlayActivated, GameOverlayActivated_t, OnGameOverlayActivatedCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameRichPresenceJoinRequested, GameRichPresenceJoinRequested_t, OnGameRichPresenceJoinRequestedCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnGameServerChangeRequested, GameServerChangeRequested_t, OnGameServerChangeRequestedCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnJoinClanChatRoomCompletionResult, JoinClanChatRoomCompletionResult_t, OnJoinClanChatRoomCompletionResultCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnPersonaStateChange, PersonaStateChange_t, OnPersonaStateChangeCallback);
	STEAM_CALLBACK_MANUAL(USteamFriends, OnSetPersonaNameResponse, SetPersonaNameResponse_t, OnSetPersonaNameResponseCallback);
};

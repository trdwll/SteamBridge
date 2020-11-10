// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnFriendsEnumerateFollowingListDelegate, ESteamResult, Result, TArray<FSteamID>, SteamIDs, int32, ResultsReturned, int32, TotalResults);
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

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "Steam Friends", CompactNodeTitle = "SteamFriends"))
	static USteamFriends* GetFriends() { return USteamFriends::StaticClass()->GetDefaultObject<USteamFriends>(); }

	/**
	 * Activates the Steam Overlay to a specific dialog.
	 *
	 * @param const ESteamOverlayTypes OverlayType
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlay(const ESteamGameOverlayTypes OverlayType);

	/**
	 * Activates the Steam Overlay to open the invite dialog. Invitations sent from this dialog will be for the provided lobby.
	 *
	 * @param FSteamID SteamIDLobby
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayInviteDialog(FSteamID SteamIDLobby) { SteamFriends()->ActivateGameOverlayInviteDialog(SteamIDLobby.Value); }

	/**
	 * Activates the Steam Overlay to the Steam store page for the provided app.
	 * Using k_uAppIdInvalid brings the user to the front page of the Steam store.
	 *
	 * @param int32 AppID
	 * @param ESteamOverlayToStoreFlag eFlag
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToStore(int32 AppID, ESteamOverlayToStoreFlag eFlag) { SteamFriends()->ActivateGameOverlayToStore(AppID, (EOverlayToStoreFlag)eFlag); }

	/**
	 * Activates Steam Overlay to a specific dialog.
	 *
	 * @param const ESteamGameUserOverlayTypes OverlayType
	 * @param FSteamID SteamID
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToUser(const ESteamGameUserOverlayTypes OverlayType, FSteamID SteamID);

	/**
	 * Activates Steam Overlay web browser directly to the specified URL.
	 *
	 * @param const FString & URL
	 * @param bool bShowModal
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void ActivateGameOverlayToWebPage(const FString& URL, bool bShowModal = false) const { SteamFriends()->ActivateGameOverlayToWebPage(TCHAR_TO_ANSI(*URL), bShowModal ? k_EActivateGameOverlayToWebPageMode_Modal : k_EActivateGameOverlayToWebPageMode_Default); }

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
	 * @param FSteamID SteamIDClanChat
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool CloseClanChatWindowInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->CloseClanChatWindowInSteam(SteamIDClanChat.Value); }

	/**
	 * Refresh the Steam Group activity data or get the data from groups other than one that the current user is a member.
	 * After receiving the callback you can then use GetClanActivityCounts to get the up to date user counts.
	 *
	 * @param TArray<FSteamID> & SteamClanIDs
	 * @param int32 ClansToRequest
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall DownloadClanActivityCounts(TArray<FSteamID>& SteamClanIDs, int32 ClansToRequest = 1) const;

	#if 0
UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall EnumerateFollowingList() const { return SteamFriends()->EnumerateFollowingList(0); }
#endif // 0

	/**
	 * Gets the Steam ID at the given index in a Steam group chat.
	 * You must call GetClanChatMemberCount before calling this.
	 *
	 * @param FSteamID SteamIDClan
	 * @param int32 UserIndex
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetChatMemberByIndex(FSteamID SteamIDClan, int32 UserIndex) const { return SteamFriends()->GetChatMemberByIndex(SteamIDClan.Value, UserIndex).ConvertToUint64(); }

	/**
	 * Gets the most recent information we have about what the users in a Steam Group are doing.
	 * This can only retrieve data that the local client knows about. To refresh the data or get data from a group other than one that the current user is a member of you must call DownloadClanActivityCounts.
	 *
	 * @param FSteamID SteamIDClan
	 * @param int32 & Online
	 * @param int32 & InGame
	 * @param int32 & Chatting
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool GetClanActivityCounts(FSteamID SteamIDClan, int32& Online, int32& InGame, int32& Chatting) { return SteamFriends()->GetClanActivityCounts(SteamIDClan.Value, &Online, &InGame, &Chatting); }

	/**
	 * Gets the Steam group's Steam ID at the given index.
	 *
	 * @param int32 ClanIndex
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
	 * @param FSteamID  SteamIDClan
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanChatMemberCount(FSteamID SteamIDClan) const { return SteamFriends()->GetClanChatMemberCount(SteamIDClan.Value); }

	/**
	 * Gets the data from a Steam group chat room message.
	 * This should only ever be called in response to a GameConnectedClanChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDClanChat
	 * @param int32 MessageID
	 * @param FString & Message
	 * @param ESteamChatEntryType & ChatEntryType
	 * @param FSteamID & SteamIDChatter
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanChatMessage(FSteamID SteamIDClanChat, int32 MessageID, FString& Message, ESteamChatEntryType& ChatEntryType, FSteamID& SteamIDChatter);

	/**
	 * Gets the number of Steam groups that the current user is a member of.
	 * This is used for iteration, after calling this then GetClanByIndex can be used to get the Steam ID of each Steam group.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanCount() const { return SteamFriends()->GetClanCount(); }

	/**
	 * Gets the display name for the specified Steam group; if the local client knows about it.
	 *
	 * @param FSteamID SteamIDClan
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetClanName(FSteamID SteamIDClan) const { return SteamFriends()->GetClanName(SteamIDClan.Value); }

	/**
	 * Gets the Steam ID of the officer at the given index in a Steam group.
	 * You must call GetClanOfficerCount before calling this.
	 *
	 * @param FSteamID SteamIDClan
	 * @param int32 OfficerIndex
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetClanOfficerByIndex(FSteamID SteamIDClan, int32 OfficerIndex) const { return SteamFriends()->GetClanOfficerByIndex(SteamIDClan.Value, OfficerIndex).ConvertToUint64(); }

	/**
	 * Gets the number of officers (administrators and moderators) in a specified Steam group.
	 * This also includes the owner of the Steam group.
	 * This is used for iteration, after calling this then GetClanOfficerByIndex can be used to get the Steam ID of each officer.
	 * You must call RequestClanOfficerList before this to get the required data!
	 *
	 * @param FSteamID SteamIDClan
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetClanOfficerCount(FSteamID SteamIDClan) const { return SteamFriends()->GetClanOfficerCount(SteamIDClan.Value); }

	/**
	 * Gets the owner of a Steam Group.
	 * You must call RequestClanOfficerList before this to get the required data!
	 *
	 * @param FSteamID SteamIDClan
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetClanOwner(FSteamID SteamIDClan) const { return SteamFriends()->GetClanOwner(SteamIDClan.Value).ConvertToUint64(); }

	/**
	 * Gets the unique tag (abbreviation) for the specified Steam group; If the local client knows about it.
	 * The Steam group abbreviation is a unique way for people to identify the group and is limited to 12 characters. In some games this will appear next to the name of group members.
	 *
	 * @param FSteamID SteamIDClan
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetClanTag(FSteamID SteamIDClan) const { return SteamFriends()->GetClanTag(SteamIDClan.Value); }

	/**
	 * Gets the Steam ID of the recently played with user at the given index.
	 * You must call GetCoplayFriendCount before calling this.
	 *
	 * @param int32 CoplayFriendIndex
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetCoplayFriend(int32 CoplayFriendIndex) const { return SteamFriends()->GetCoplayFriend(CoplayFriendIndex).ConvertToUint64(); }

	/**
	 * Gets the number of players that the current users has recently played with, across all games.
	 * This is used for iteration, after calling this then GetCoplayFriend can be used to get the Steam ID of each player.
	 * These players are have been set with previous calls to SetPlayedWith.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetCoplayFriendCount() const { return SteamFriends()->GetCoplayFriendCount(); }

	/**
	 * Gets the number of users following the specified user.
	 *
	 * @param FSteamID SteamID
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFollowerCount(FSteamID SteamID) const { return SteamFriends()->GetFollowerCount(SteamID.Value); }

	/**
	 * Gets the Steam ID of the user at the given index.
	 * You must call GetFriendCount before calling this.
	 *
	 * @param int32 FriendIndex
	 * @param const TArray<ESteamFriendFlags> & FriendFlags
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetFriendByIndex(int32 FriendIndex, const TArray<ESteamFriendFlags>& FriendFlags) const;

	/**
	 * Gets the app ID of the game that user played with someone on their recently-played-with list.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCoplayGame(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendCoplayGame(SteamIDFriend.Value); }

	/**
	 * Gets the number of users the client knows about who meet a specified criteria. (Friends, blocked, users on the same server, etc)
	 * This can be used to iterate over all of the users by calling GetFriendByIndex to get the Steam IDs of each user.
	 *
	 * @param const TArray<ESteamFriendFlags> & FriendFlags
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCount(const TArray<ESteamFriendFlags>& FriendFlags) const;

	/**
	 * Get the number of users in a source (Steam group, chat room, lobby, or game server).
	 * Large Steam groups cannot be iterated by the local user.
	 * If you're getting the number of lobby members then you should use ISteamMatchmaking::GetNumLobbyMembers instead.
	 * This is used for iteration, after calling this then GetFriendFromSourceByIndex can be used to get the Steam ID of each person in the source.
	 *
	 * @param const FSteamID & SteamIDSource
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendCountFromSource(FSteamID SteamIDSource) const { return SteamFriends()->GetFriendCountFromSource(SteamIDSource.Value); }

	/**
	 * Gets the Steam ID at the given index from a source (Steam group, chat room, lobby, or game server).
	 * You must call GetFriendCountFromSource before calling this.
	 *
	 * @param FSteamID SteamIDSource
	 * @param int32 FriendIndex
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamID GetFriendFromSourceByIndex(FSteamID SteamIDSource, int32 FriendIndex) const { return SteamFriends()->GetFriendFromSourceByIndex(SteamIDSource.Value, FriendIndex).ConvertToUint64(); }

	/**
	 * Checks if the specified friend is in a game, and gets info about the game if they are.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param FSteamID & GameID
	 * @param FString & GameIP
	 * @param int32 & GamePort
	 * @param int32 & QueryPort
	 * @param FSteamID & SteamIDLobby
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool GetFriendGamePlayed(FSteamID SteamIDFriend, FSteamID& GameID, FString& GameIP, int32& GamePort, int32& QueryPort, FSteamID& SteamIDLobby);

	/**
	 * Gets the data from a Steam friends message.
	 * This should only ever be called in response to a GameConnectedFriendChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param int32 MessageIndex
	 * @param FString & Message
	 * @param ESteamChatEntryType & ChatEntryType
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendMessage(FSteamID SteamIDFriend, int32 MessageIndex, FString& Message, ESteamChatEntryType& ChatEntryType);

	/**
	 * Gets the specified user's persona (display) name.
	 * This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user.
	 * Upon on first joining a lobby, chat room, or game server the current user will not known the name of the other users automatically; that information will arrive asynchronously via PersonaStateChange_t callbacks.
	 * To get the persona name of the current user use GetPersonaName.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendPersonaName(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendPersonaName(SteamIDFriend.Value); }

	/**
	 * Gets one of the previous display names for the specified user.
	 * This only works for display names that the current user has seen on the local computer.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param int32 NameHistoryIndex
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendPersonaNameHistory(FSteamID SteamIDFriend, int32 NameHistoryIndex = 0) const { return SteamFriends()->GetFriendPersonaNameHistory(SteamIDFriend.Value, NameHistoryIndex); }

	/**
	 * Gets the current status of the specified user.
	 * This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user.
	 * To get the state of the current user use GetPersonaState.
	 *
	 * @param FString SteamIDFriend
	 * @return ESteamPersonaState
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamPersonaState GetFriendPersonaState(FSteamID SteamIDFriend) const { return (ESteamPersonaState)SteamFriends()->GetFriendPersonaState(SteamIDFriend.Value); }

	/**
	 * Gets a relationship to a specified user.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return ESteamFriendRelationship
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamFriendRelationship GetFriendRelationship(FSteamID SteamIDFriend) const { return (ESteamFriendRelationship)SteamFriends()->GetFriendRelationship(SteamIDFriend.Value); }

	/**
	 * Get a Rich Presence value from a specified friend.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param const FString & Key
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendRichPresence(FSteamID SteamIDFriend, const FString& Key) const { return SteamFriends()->GetFriendRichPresence(SteamIDFriend.Value, TCHAR_TO_ANSI(*Key)); }

	/**
	 * Get a Rich Presence value from a specified friend by index.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param int32 KeyIndex
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendRichPresenceKeyByIndex(FSteamID SteamIDFriend, int32 KeyIndex) const { return SteamFriends()->GetFriendRichPresenceKeyByIndex(SteamIDFriend.Value, KeyIndex); }

	/**
	 * Gets the number of Rich Presence keys that are set on the specified user.
	 * This is used for iteration, after calling this then GetFriendRichPresenceKeyByIndex to get the rich presence keys.
	 * This is typically only ever used for debugging purposes.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendRichPresenceKeyCount(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendRichPresenceKeyCount(SteamIDFriend.Value); }

	/**
	 * Gets the number of friends groups (tags) the user has created.
	 * This is used for iteration, after calling this then GetFriendsGroupIDByIndex can be used to get the ID of each friend group.
	 * This is not to be confused with Steam groups. Those can be obtained with GetClanCount.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendsGroupCount() const { return SteamFriends()->GetFriendsGroupCount(); }

	/**
	 * Gets the friends group ID for the given index.
	 * You must call GetFriendsGroupCount before calling this.
	 *
	 * @param int32 GroupIndex
	 * @return FSteamFriendsGroupID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamFriendsGroupID GetFriendsGroupIDByIndex(int32 GroupIndex = 0) const { return SteamFriends()->GetFriendsGroupIDByIndex(GroupIndex); }

	/**
	 * Gets the number of friends in a given friends group.
	 * This should be called before getting the list of friends with GetFriendsGroupMembersList.
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendsGroupMembersCount(FSteamFriendsGroupID FriendsGroupID) const { return SteamFriends()->GetFriendsGroupMembersCount(FriendsGroupID.Value); }

	/**
	 * Gets the number of friends in the given friends group.
	 * If fewer friends exist than requested those positions' Steam IDs will be invalid.
	 * You must call GetFriendsGroupMembersCount before calling this to set up the pOutSteamIDMembers array with an appropriate size!
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID
	 * @param TArray<FSteamID> & MemberSteamIDs
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void GetFriendsGroupMembersList(FSteamFriendsGroupID FriendsGroupID, TArray<FSteamID>& MemberSteamIDs);

	/**
	 * Gets the name for the given friends group.
	 *
	 * @param FSteamFriendsGroupID FriendsGroupID
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetFriendsGroupName(FSteamFriendsGroupID FriendsGroupID) const { return SteamFriends()->GetFriendsGroupName(FriendsGroupID.Value); }

	/**
	 * Gets the Steam level of the specified user.
	 * You can use the local users Steam ID (ISteamUser::GetSteamID) to get their level.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	int32 GetFriendSteamLevel(FSteamID SteamIDFriend) const { return SteamFriends()->GetFriendSteamLevel(SteamIDFriend.Value); }

	/**
	 * Gets a handle to the avatar for the specified user.
	 * You can pass in ISteamUser::GetSteamID to get the current users avatar.
	 * This only works for users that the local user knows about. They will automatically know about their friends, people on leaderboards they've requested, or people in the same source as them -
	 * (Steam group, chat room, lobby, or game server). If they don't know about them then you must call RequestUserInformation to cache the avatar locally.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param ESteamAvatarSize AvatarSize
	 * @return UTexture2D*
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	UTexture2D* GetFriendAvatar(FSteamID SteamIDFriend, ESteamAvatarSize AvatarSize) const;

	/**
	 * Gets the current users persona (display) name.
	 * This is the same name that is displayed the users community profile page.
	 * To get the persona name of other users use GetFriendPersonaName.
	 *
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetPersonaName() const { return SteamFriends()->GetPersonaName(); }

	/**
	 * Gets the friend status of the current user.
	 * To get the state of other users use GetFriendPersonaState.
	 *
	 * @return ESteamPersonaState
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	ESteamPersonaState GetPersonaState() const { return (ESteamPersonaState)SteamFriends()->GetPersonaState(); }

	/**
	 * Gets the nickname that the current user has set for the specified user.
	 *
	 * @param FSteamID SteamIDPlayer
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetPlayerNickname(FSteamID SteamIDPlayer) const { return SteamFriends()->GetPlayerNickname(SteamIDPlayer.Value); }

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
	 * @param FSteamID SteamIDFriend
	 * @param const TArray<ESteamFriendFlags> & FriendFlags
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool HasFriend(FSteamID SteamIDFriend, const TArray<ESteamFriendFlags>& FriendFlags);

	/**
	 * Invites a friend or clan member to the current game using a special invite string.
	 * If the target user accepts the invite then the pchConnectString gets added to the command-line when launching the game.
	 * If the game is already running for that user, then they will receive a GameRichPresenceJoinRequested_t callback with the connect string.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param const FString & ConnectString
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool InviteUserToGame(FSteamID SteamIDFriend, const FString& ConnectString) const { return SteamFriends()->InviteUserToGame(SteamIDFriend.Value, TCHAR_TO_UTF8(*ConnectString)); }

	/**
	 * Checks if a user in the Steam group chat room is an admin.
	 *
	 * @param FSteamID SteamIDClanChat
	 * @param FSteamID SteamIDUser
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanChatAdmin(FSteamID SteamIDClanChat, FSteamID SteamIDUser) const { return SteamFriends()->IsClanChatAdmin(SteamIDClanChat.Value, SteamIDUser.Value); }

	/**
	 * Checks if the Steam group is public.
	 *
	 * @param FSteamID SteamIDClan
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanPublic(FSteamID SteamIDClan) const { return SteamFriends()->IsClanPublic(SteamIDClan.Value); }

	/**
	 * Checks if the Steam group is an official game group/community hub.
	 *
	 * @param FSteamID SteamIDClan
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanOfficialGameGroup(FSteamID SteamIDClan) const { return SteamFriends()->IsClanOfficialGameGroup(SteamIDClan.Value); }

	/**
	 * Checks if the Steam Group chat room is open in the Steam UI.
	 *
	 * @param FSteamID SteamIDClanChat
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsClanChatWindowOpenInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->IsClanChatWindowOpenInSteam(SteamIDClanChat.Value); }

	/**
	 * Checks if the current user is following the specified user.
	 *
	 * @param FSteamID SteamID
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall IsFollowing(FSteamID SteamID) const { return SteamFriends()->IsFollowing(SteamID.Value); }

	/**
	 * Checks if a specified user is in a source (Steam group, chat room, lobby, or game server).
	 *
	 * @param FSteamID SteamIDUser
	 * @param FSteamID SteamIDSource
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool IsUserInSource(FSteamID SteamIDUser, FSteamID SteamIDSource) const { return SteamFriends()->IsUserInSource(SteamIDUser.Value, SteamIDSource.Value); }

	/**
	 * Allows the user to join Steam group (clan) chats right within the game.
	 * The behavior is somewhat complicated, because the user may or may not be already in the group chat from outside the game or in the overlay.
	 * You can use ActivateGameOverlayToUser to open the in-game overlay version of the chat.
	 * If you have joined a Steam group chat then you should be watching for the following callbacks:
	 *  GameConnectedClanChatMsg_t
	 *  GameConnectedChatJoin_t
	 *  GameConnectedChatLeave_t
	 *
	 * @param FSteamID SteamIDClan
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall JoinClanChatRoom(FSteamID SteamIDClan) const { return SteamFriends()->JoinClanChatRoom(SteamIDClan.Value); }

	/**
	 * Leaves a Steam group chat that the user has previously entered with JoinClanChatRoom.
	 *
	 * @param FSteamID SteamIDClan
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool LeaveClanChatRoom(FSteamID SteamIDClan) const { return SteamFriends()->LeaveClanChatRoom(SteamIDClan.Value); }

	/**
	 * Opens the specified Steam group chat room in the Steam UI.
	 *
	 * @param FSteamID SteamIDClanChat
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool OpenClanChatWindowInSteam(FSteamID SteamIDClanChat) const { return SteamFriends()->OpenClanChatWindowInSteam(SteamIDClanChat.Value); }

	/**
	 * Sends a message to a Steam friend.
	 *
	 * @param FSteamID SteamIDFriend
	 * @param const FString & MsgToSend
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool ReplyToFriendMessage(FSteamID SteamIDFriend, const FString& MsgToSend) const { return SteamFriends()->ReplyToFriendMessage(SteamIDFriend.Value, TCHAR_TO_UTF8(*MsgToSend)); }

	/**
	 * Requests information about a Steam group officers (administrators and moderators).
	 * You can only ask about Steam groups that a user is a member of.
	 * This won't download avatars for the officers automatically. If no avatar image is available for an officer, then call RequestUserInformation to download the avatar.
	 *
	 * @param FSteamID SteamIDClan
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall RequestClanOfficerList(FSteamID SteamIDClan) const { return SteamFriends()->RequestClanOfficerList(SteamIDClan.Value); }

	/**
	 * Requests Rich Presence data from a specific user.
	 * This is used to get the Rich Presence information from a user that is not a friend of the current user, like someone in the same lobby or game server.
	 * This function is rate limited, if you call this too frequently for a particular user then it will just immediately post a callback without requesting new data from the server.
	 *
	 * @param FSteamID SteamIDFriend
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void RequestFriendRichPresence(FSteamID SteamIDFriend) { SteamFriends()->RequestFriendRichPresence(SteamIDFriend.Value); }

	/**
	 * Requests the persona name and optionally the avatar of a specified user.
	 * It's a lot slower to download avatars and churns the local cache, so if you don't need avatars, don't request them.
	 *
	 * @param FSteamID SteamIDUser
	 * @param bool bRequireNameOnly
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool RequestUserInformation(FSteamID SteamIDUser, bool bRequireNameOnly) const { return SteamFriends()->RequestUserInformation(SteamIDUser.Value, bRequireNameOnly); }

	/**
	 * Sends a message to a Steam group chat room.
	 *
	 * @param FSteamID SteamIDClanChat
	 * @param const FString & Text
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SendClanChatMessage(FSteamID SteamIDClanChat, const FString& Text) const { return SteamFriends()->SendClanChatMessage(SteamIDClanChat.Value, TCHAR_TO_UTF8(*Text)); }

	/**
	 * Let Steam know that the user is currently using voice chat in game.
	 * This will suppress the microphone for all voice communication in the Steam UI.
	 *
	 * @param FSteamID SteamIDUser
	 * @param bool bSpeaking
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void SetInGameVoiceSpeaking(FSteamID SteamIDUser, bool bSpeaking) { SteamFriends()->SetInGameVoiceSpeaking(SteamIDUser.Value, bSpeaking); }

	/**
	 * Listens for Steam friends chat messages.
	 * You can then show these chats inline in the game. For example with a Blizzard style chat message system or the chat system in Dota 2.
	 * After enabling this you will receive GameConnectedFriendChatMsg_t callbacks when ever the user receives a chat message. You can get the actual message data from this callback with GetFriendMessage. You can send messages with ReplyToFriendMessage.
	 *
	 * @param bool bInterceptEnabled
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SetListenForFriendsMessages(bool bInterceptEnabled) const { return SteamFriends()->SetListenForFriendsMessages(bInterceptEnabled); }

	/**
	 * Sets the current users persona name, stores it on the server and publishes the changes to all friends who are online.
	 * Changes take place locally immediately, and a PersonaStateChange_t callback is posted, presuming success.
	 * If the name change fails to happen on the server, then an additional PersonaStateChange_t callback will be posted to change the name back, in addition to the final result available in the call result.
	 *
	 * @param const FString & PersonaName
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FSteamAPICall SetPersonaName(const FString& PersonaName) const { return SteamFriends()->SetPersonaName(TCHAR_TO_UTF8(*PersonaName)); }

	/**
	 * Mark a target user as 'played with'.
	 * The current user must be in game with the other player for the association to work.
	 *
	 * @param FSteamID SteamIDUserPlayedWith
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	void SetPlayedWith(FSteamID SteamIDUserPlayedWith) { SteamFriends()->SetPlayedWith(SteamIDUserPlayedWith.Value); }

	/**
	 * Sets a Rich Presence key/value for the current user that is automatically shared to all friends playing the same game.
	 *
	 * @param const FString & Key
	 * @param const FString & Value
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	bool SetRichPresence(const FString& Key, const FString& Value) const { return SteamFriends()->SetRichPresence(TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnAvatarImageLoaded"))
	FOnAvatarImageLoadedDelegate m_OnAvatarImageLoaded;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnClanOfficerListResponse"))
	FOnClanOfficerListResponseDelegate m_OnClanOfficerListResponse;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnDownloadClanActivityCountsResult"))
	FOnDownloadClanActivityCountsResultDelegate m_OnDownloadClanActivityCountsResult;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendRichPresenceUpdate"))
	FOnFriendRichPresenceUpdateDelegate m_OnFriendRichPresenceUpdate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsEnumerateFollowingList"))
	FOnFriendsEnumerateFollowingListDelegate m_OnFriendsEnumerateFollowingList;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsGetFollowerCount"))
	FOnFriendsGetFollowerCountDelegate m_OnFriendsGetFollowerCount;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnFriendsIsFollowing"))
	FOnFriendsIsFollowingDelegate m_OnFriendsIsFollowing;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedChatJoin"))
	FOnGameConnectedChatJoinDelegate m_OnGameConnectedChatJoin;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedChatLeave"))
	FOnGameConnectedChatLeaveDelegate m_OnGameConnectedChatLeave;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedClanChatMsg"))
	FOnGameConnectedClanChatMsgDelegate m_OnGameConnectedClanChatMsg;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameConnectedFriendChatMsg"))
	FOnGameConnectedFriendChatMsgDelegate m_OnGameConnectedFriendChatMsg;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameLobbyJoinRequested"))
	FOnGameLobbyJoinRequestedDelegate m_OnGameLobbyJoinRequested;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameOverlayActivated"))
	FOnGameOverlayActivatedDelegate m_OnGameOverlayActivated;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameRichPresenceJoinRequested"))
	FOnGameRichPresenceJoinRequestedDelegate m_OnGameRichPresenceJoinRequested;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnGameServerChangeRequested"))
	FOnGameServerChangeRequestedDelegate m_OnGameServerChangeRequested;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnJoinClanChatRoomCompletionResult"))
	FOnJoinClanChatRoomCompletionResultDelegate m_OnJoinClanChatRoomCompletionResult;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnPersonaStateChange"))
	FOnPersonaStateChangeDelegate m_OnPersonaStateChange;

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

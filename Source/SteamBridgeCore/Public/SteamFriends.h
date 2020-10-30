// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamFriends.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSteamPersonaStateChangeDelegate, FSteamID, SteamID, ESteamPersonaChange, PersonaStateChange);

/**
 * Functions for accessing and manipulating Steam friends information.
 * https://partner.steamgames.com/doc/api/ISteamFriends
 */
UCLASS()
class STEAMBRIDGECORE_API USteamFriends final : public UObject
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
	void ActivateGameOverlayInviteDialog( FSteamID SteamIDLobby) { SteamFriends()->ActivateGameOverlayInviteDialog(SteamIDLobby.Value); }

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

	// FSteamAPICall DownloadClanActivityCounts(TArray<FSteamID>& SteamClanIDs, int32 ClansToRequest = 1);
	// FSteamAPICall EnumerateFollowingList();

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



	// TODO: Pickup on GetLargeFriendAvatar (going to bed ;))









	/** Get the persona of the current user. */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends")
	FString GetPersonaName() const;

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Friends")
	bool RequestUserInformation(FSteamID SteamID, bool bRequireNameOnly);

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnSteamPersonaStateChanged"))
	FOnSteamPersonaStateChangeDelegate m_SteamPersonaStateChangeCallback;

private:
	STEAM_CALLBACK_MANUAL(USteamFriends, OnSteamPersonaStateChange, PersonaStateChange_t, OnSteamPersonaStateChangeCallback);
};

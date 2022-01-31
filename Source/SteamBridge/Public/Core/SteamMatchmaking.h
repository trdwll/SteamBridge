// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamMatchmaking.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFavoritesListAccountsUpdatedDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnFavoritesListChangedDelegate, FString, IP, int32, QueryPort, int32, ConnectionPort, int32, AppID, const TArray<ESteamFavoriteFlags>&, Flags, bool, bAdd, FAccountID, AccountID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLobbyChatMsgDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDUser, ESteamChatEntryType, ChatEntryType, int32, ChatID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLobbyChatUpdateDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDUserChanged, FSteamID, SteamIDMakingChange, const TArray<ESteamChatMemberStateChange>&, ChatMemberStateChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLobbyCreatedDelegate, ESteamResult, Result, FSteamID, SteamIDLobby);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLobbyDataUpdateDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDMember, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLobbyEnterDelegate, FSteamID, SteamIDLobby, bool, bLocked, ESteamChatRoomEnterResponse, ChatRoomEnterResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLobbyGameCreatedDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDGameServer, FString, IP, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLobbyInviteDelegate, FSteamID, SteamIDUser, FSteamID, SteamIDLobby, int32, GameID);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyKickedDelegate); // This is currently unused
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyMatchListDelegate, int32, LobbiesMatching);

/**
 * Functions for clients to access matchmaking services, favorites, and to operate on game lobbies.
 * https://partner.steamgames.com/doc/api/ISteamMatchmaking
 */
UCLASS()
class STEAMBRIDGE_API USteamMatchmaking final : public UObject
{
	GENERATED_BODY()

public:
	USteamMatchmaking();
	~USteamMatchmaking();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Matchmaking", CompactNodeTitle = "SteamMatchmaking"))
	static USteamMatchmaking* GetSteamMatchmaking() { return USteamMatchmaking::StaticClass()->GetDefaultObject<USteamMatchmaking>(); }

	/**
	 * Adds the game server to the local favorites list or updates the time played of the server if it already exists in the list.
	 *
	 * @param int32 AppID - The App ID of the game.
	 * @param const FString & IP - The IP address of the server
	 * @param int32 ConnPort - The port used to connect to the server, in host order.
	 * @param int32 QueryPort - The port used to query the server, in host order.
	 * @param const TArray<ESteamFavoriteFlags> & Flags - Sets the whether the server should be added to the favorites list or history list. See k_unFavoriteFlagNone for more information.
	 * @param const FDateTime& TimeLastPlayedOnServer - This should be the current time in a friendly format.
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	int32 AddFavoriteGame(int32 AppID, const FString& IP, int32 ConnPort, int32 QueryPort, const TArray<ESteamFavoriteFlags>& Flags, const FDateTime& TimeLastPlayedOnServer) const;

	/**
	 * Sets the physical distance for which we should search for lobbies, this is based on the users IP address and a IP location map on the Steam backed.
	 *
	 * @param ESteamLobbyDistanceFilter LobbyDistanceFilter - Specifies the maximum distance.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListDistanceFilter(ESteamLobbyDistanceFilter LobbyDistanceFilter) { SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)LobbyDistanceFilter); }

	/**
	 * Filters to only return lobbies with the specified number of open slots available.
	 *
	 * @param int32 SlotsAvailable - The number of open slots that must be open.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable) { SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(SlotsAvailable); }

	/**
	 * Sorts the results closest to the specified value.
	 * Near filters don't actually filter out values, they just influence how the results are sorted. You can specify multiple near filters, with the first near filter influencing the most, and the last near filter influencing the least.
	 *
	 * @param const FString & KeyToMatch - The filter key name to match. This can not be longer than k_nMaxLobbyKeyLength.
	 * @param int32 ValueToBeCloseTo - The value that lobbies will be sorted on.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListNearValueFilter(const FString& KeyToMatch, int32 ValueToBeCloseTo) { SteamMatchmaking()->AddRequestLobbyListNearValueFilter(TCHAR_TO_UTF8(*KeyToMatch), ValueToBeCloseTo); }

	/**
	 * Adds a numerical comparison filter to the next RequestLobbyList call.
	 *
	 * @param const FString & KeyToMatch - The filter key name to match. This can not be longer than k_nMaxLobbyKeyLength.
	 * @param int32 ValueToMatch - The number to match.
	 * @param ESteamLobbyComparison ComparisonType - The type of comparison to make.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListNumericalFilter(const FString& KeyToMatch, int32 ValueToMatch, ESteamLobbyComparison ComparisonType) { SteamMatchmaking()->AddRequestLobbyListNumericalFilter(TCHAR_TO_UTF8(*KeyToMatch), ValueToMatch, (ELobbyComparison)((uint8)ComparisonType - 2)); }

	/**
	 * Sets the maximum number of lobbies to return. The lower the count the faster it is to download the lobby results & details to the client.
	 *
	 * @param int32 MaxResults - The maximum number of lobbies to return.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListResultCountFilter(int32 MaxResults) { SteamMatchmaking()->AddRequestLobbyListResultCountFilter(MaxResults); }

	/**
	 * Adds a string comparison filter to the next RequestLobbyList call.
	 *
	 * @param const FString & KeyToMatch - The filter key name to match. This can not be longer than k_nMaxLobbyKeyLength.
	 * @param const FString & ValueToMatch - The string to match.
	 * @param ESteamLobbyComparison ComparisonType - The type of comparison to make.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListStringFilter(const FString& KeyToMatch, const FString& ValueToMatch, ESteamLobbyComparison ComparisonType) { SteamMatchmaking()->AddRequestLobbyListStringFilter(TCHAR_TO_UTF8(*KeyToMatch), TCHAR_TO_UTF8(*ValueToMatch), (ELobbyComparison)((uint8)ComparisonType - 2)); }

	/**
	 * Create a new matchmaking lobby.
	 * Triggers a LobbyEnter_t callback.
	 * Triggers a LobbyDataUpdate_t callback.
	 * If the results returned via the LobbyCreated_t call result indicate success then the lobby is joined & ready to use at this point.
	 * The LobbyEnter_t callback is also received since the local user has joined their own lobby.
	 *
	 * @param ESteamLobbyType LobbyType - The type and visibility of this lobby. This can be changed later via SetLobbyType.
	 * @param uint8 MaxMembers - The maximum number of players that can join this lobby. This can not be above 250.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a LobbyCreated_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall CreateLobby(ESteamLobbyType LobbyType = ESteamLobbyType::FriendsOnly, uint8 MaxMembers = 1) const { return SteamMatchmaking()->CreateLobby((ELobbyType)LobbyType, MaxMembers); }

	/**
	 * Removes a metadata key from the lobby.
	 * This can only be done by the owner of the lobby.
	 * This will only send the data if the key existed. There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to delete the metadata for.
	 * @param const FString & Key - The key to delete the data for.
	 * @return bool - true if the key/value was successfully deleted; otherwise, false if steamIDLobby or pchKey are invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool DeleteLobbyData(FSteamID SteamIDLobby, const FString& Key) const { return SteamMatchmaking()->DeleteLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key)); }

	/**
	 * Gets the details of the favorite game server by index.
	 * NOTE: You must call GetFavoriteGameCount before calling this.
	 *
	 * @param int32 GameIndex - The index of the favorite game server to get the details of. This must be between 0 and GetFavoriteGameCount
	 * @param int32 & AppID - Returns the App ID this server is for.
	 * @param FString & IP - Returns the IP address of the server
	 * @param int32 & ConnPort - Returns the port used to connect to the server, in host order.
	 * @param int32 & QueryPort - Returns the port used to query the server, in host order.
	 * @param TArray<ESteamFavoriteFlags> & Flags - Returns whether the server is on the favorites list or the history list. See k_unFavoriteFlagNone for more information.
	 * @param FDateTime & TimeLastPlayedOnServer - Returns the time the server was last added to the favorites list in a friendly format.
	 * @return bool - true if the details were successfully retrieved. false if iGame was an invalid index.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetFavoriteGame(int32 GameIndex, int32& AppID, FString& IP, int32& ConnPort, int32& QueryPort, TArray<ESteamFavoriteFlags>& Flags, FDateTime& TimeLastPlayedOnServer) const;

	/**
	 * Gets the number of favorite and recent game servers the user has stored locally.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetFavoriteGameCount() const { return SteamMatchmaking()->GetFavoriteGameCount(); }

	/**
	 * Gets the Steam ID of the lobby at the specified index after receiving the RequestLobbyList results.
	 * NOTE: This should only be called after a LobbyMatchList_t call result is received.
	 *
	 * @param int32 LobbyIndex - The index of the lobby to get the Steam ID of, from 0 to LobbyMatchList_t.m_nLobbiesMatching.
	 * @return FSteamID - Returns k_steamIDNil if the provided index is invalid or there are no lobbies found.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyByIndex(int32 LobbyIndex) const { return SteamMatchmaking()->GetLobbyByIndex(LobbyIndex).ConvertToUint64(); }

	/**
	 * Gets the data from a lobby chat message after receiving a LobbyChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the chat entry from. This should almost always be LobbyChatMsg_t::m_ulSteamIDUser.
	 * @param int32 ChatID - The index of the chat entry in the lobby. This should almost always be LobbyChatMsg_t::m_iChatID.
	 * @param FSteamID & SteamIDUser - If set then this returns the Steam ID of the user who sent this message. Usually unneeded since it will be the same as LobbyChatMsg_t::m_ulSteamIDUser.
	 * @param FString & Message - Returns the message data by copying it into this buffer. This buffer should be up to 4 Kilobytes.
	 * @param ESteamChatEntryType & ChatEntryType - If set then this will just always return k_EChatEntryTypeChatMsg. This can usually just be set to NULL.
	 * @return int32 - The number of bytes copied into pvData.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyChatEntry(FSteamID SteamIDLobby, int32 ChatID, FSteamID& SteamIDUser, FString& Message, ESteamChatEntryType& ChatEntryType) const;

	/**
	 * Gets the metadata associated with the specified key from the specified lobby.
	 * NOTE: This can only get metadata from lobbies that the client knows about, either after receiving a list of lobbies from LobbyMatchList_t, retrieving the data with RequestLobbyData or after joining a lobby.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the metadata from.
	 * @param const FString & Key - The key to get the value of.
	 * @return FString - Returns an empty string ("") if no value is set for this key, or if steamIDLobby is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FString GetLobbyData(FSteamID SteamIDLobby, const FString& Key) const { return UTF8_TO_TCHAR(SteamMatchmaking()->GetLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key))); }

	/**
	 * Gets a lobby metadata key/value pair by index.
	 * NOTE: You must call GetLobbyDataCount before calling this.
	 *
	 * @param FSteamID SteamIDLobby - This MUST be the same lobby used in the previous call to GetLobbyDataCount!
	 * @param int32 LobbyData - An index between 0 and GetLobbyDataCount.
	 * @param FString & Key - Returns the name of the key at the specified index by copying it into this buffer.
	 * @param FString & Value - Returns the value associated with the key at the specified index by copying it into this buffer.
	 * @return bool - true upon success; otherwise, false if the steamIDLobby or iLobbyData are invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetLobbyDataByIndex(FSteamID SteamIDLobby, int32 LobbyData, FString& Key, FString& Value) const;

	/**
	 * Gets the number of metadata keys set on the specified lobby.
	 * This is used for iteration, after calling this then GetLobbyDataByIndex can be used to get the key/value pair of each piece of metadata.
	 * NOTE: This can only get metadata from lobbies that the client knows about, either after receiving a list of lobbies from LobbyMatchList_t, retrieving the data with RequestLobbyData or after joining a lobby.
	 * NOTE: This should typically only ever be used for debugging purposes.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the data count from.
	 * @return int32 - Returns 0 if steamIDLobby is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyDataCount(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyDataCount(SteamIDLobby); }

	/**
	 * Gets the details of a game server set in a lobby.
	 * Either the IP/Port or the Steam ID of the game server has to be valid, depending on how you want the clients to be able to connect.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the game server information from.
	 * @param FString & GameServerIP - Returns the IP address of the game server
	 * @param int32 & GameServerPort - Returns the connection port of the game server, in host order, if it's set.
	 * @param FSteamID & SteamIDGameServer - Returns the Steam ID of the game server, if it's set.
	 * @return bool - true if the lobby is valid and has a valid game server set; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetLobbyGameServer(FSteamID SteamIDLobby, FString& GameServerIP, int32& GameServerPort, FSteamID& SteamIDGameServer) const;

	/**
	 * Gets the Steam ID of the lobby member at the given index.
	 * NOTE: You must call GetNumLobbyMembers before calling this.
	 * NOTE: The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.
	 *
	 * @param FSteamID SteamIDLobby - This MUST be the same lobby used in the previous call to GetNumLobbyMembers!
	 * @param int32 MemberIndex - An index between 0 and GetNumLobbyMembers.
	 * @return FSteamID - Invalid indices return k_steamIDNil.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyMemberByIndex(FSteamID SteamIDLobby, int32 MemberIndex) const { return SteamMatchmaking()->GetLobbyMemberByIndex(SteamIDLobby, MemberIndex).ConvertToUint64(); }

	/**
	 * Gets per-user metadata from another player in the specified lobby.
	 * This can only be queried from members in lobbies that you are currently in.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby that the other player is in.
	 * @param FSteamID SteamIDUser - The Steam ID of the player to get the metadata from.
	 * @param const FString & Key - The key to get the value of.
	 * @return FString - Returns NULL if steamIDLobby is invalid, or steamIDUser is not in the lobby. Returns an empty string ("") if pchKey is not set for the player.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FString GetLobbyMemberData(FSteamID SteamIDLobby, FSteamID SteamIDUser, const FString& Key) const { return UTF8_TO_TCHAR(SteamMatchmaking()->GetLobbyMemberData(SteamIDLobby, SteamIDUser, TCHAR_TO_UTF8(*Key))); }

	/**
	 * The current limit on the # of users who can join the lobby. Returns 0 if no limit is defined.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the member limit of.
	 * @return int32 - Returns 0 if no metadata is available for the specified lobby.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyMemberLimit(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyMemberLimit(SteamIDLobby); }

	/**
	 * Returns the current lobby owner.
	 * NOTE: You must be a member of the lobby to access this.
	 * There always one lobby owner - if the current owner leaves, another user in the lobby will become the owner automatically. It is possible (but rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the owner of.
	 * @return FSteamID - Returns k_steamIDNil if you're not in the lobby.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyOwner(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyOwner(SteamIDLobby).ConvertToUint64(); }

	/**
	 * Gets the number of users in a lobby.
	 * NOTE: The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.
	 * This is used for iteration, after calling this then GetLobbyMemberByIndex can be used to get the Steam ID of each person in the lobby. Persona information for other lobby members (name, avatar, etc.)
	 * is automatically received and accessible via the ISteamFriends interface.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to get the number of members of.
	 * @return int32 - The number of members in the lobby, 0 if the current user has no data from the lobby.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetNumLobbyMembers(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetNumLobbyMembers(SteamIDLobby); }

	/**
	 * Invite another user to the lobby.
	 * If the specified user clicks the join link, a GameLobbyJoinRequested_t callback will be posted if the user is in-game,
	 * or if the game isn't running yet then the game will be automatically launched with the command line parameter +connect_lobby <64-bit lobby Steam ID> instead.
	 * NOTE: This call doesn't check if the other user was successfully invited.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to invite the user to.
	 * @param FSteamID SteamIDInvitee - The Steam ID of the person who will be invited.
	 * @return bool - true if the invite was successfully sent; otherwise, false if the local user isn't in a lobby, no connection to Steam could be made, or the specified user is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool InviteUserToLobby(FSteamID SteamIDLobby, FSteamID SteamIDInvitee) const { return SteamMatchmaking()->InviteUserToLobby(SteamIDLobby, SteamIDInvitee); }

	/**
	 * Joins an existing lobby.
	 * The lobby Steam ID can be obtained either from a search with RequestLobbyList, joining on a friend, or from an invite.
	 * Triggers a LobbyDataUpdate_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to join.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a LobbyEnter_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall JoinLobby(FSteamID SteamIDLobby) const { return SteamMatchmaking()->JoinLobby(SteamIDLobby); }

	/**
	 * Leave a lobby that the user is currently in; this will take effect immediately on the client side, other users in the lobby will be notified by a LobbyChatUpdate_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The lobby to leave.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void LeaveLobby(FSteamID SteamIDLobby) { SteamMatchmaking()->LeaveLobby(SteamIDLobby); }

	/**
	 * Removes the game server from the local favorites list.
	 *
	 * @param int32 AppID - The App ID of the game.
	 * @param const FString & IP - 	The IP address of the server
	 * @param int32 ConnPort - The port used to connect to the server, in host order.
	 * @param int32 QueryPort - The port used to query the server, in host order.
	 * @param const TArray<ESteamFavoriteFlags> & Flags - Whether the server is on the favorites list or history list. See k_unFavoriteFlagNone for more information.
	 * @return bool - true if the server was removed; otherwise, false if the specified server was not on the users local favorites list.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool RemoveFavoriteGame(int32 AppID, const FString& IP, int32 ConnPort, int32 QueryPort, const TArray<ESteamFavoriteFlags>& Flags) const;

	/**
	 * Refreshes all of the metadata for a lobby that you're not in right now.
	 * You will never do this for lobbies you're a member of, that data will always be up to date. You can use this to refresh lobbies that you have obtained from RequestLobbyList or that are available via friends.
	 * If the specified lobby doesn't exist, LobbyDataUpdate_t::m_bSuccess will be set to false.
	 * Triggers a LobbyDataUpdate_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to refresh the metadata of.
	 * @return bool - true if the request was successfully sent to the server. false if no connection to Steam could be made, or steamIDLobby is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool RequestLobbyData(FSteamID SteamIDLobby) const { return SteamMatchmaking()->RequestLobbyData(SteamIDLobby); }

	/**
	 * Get a filtered list of relevant lobbies.
	 * There can only be one active lobby search at a time. The old request will be canceled if a new one is started. Depending on the users connection to the Steam back-end, this call can take from -
	 * 300ms to 5 seconds to complete, and has a timeout of 20 seconds.
	 * NOTE: To filter the results you MUST call the AddRequestLobbyList* functions before calling this. The filters are cleared on each call to this function.
	 * NOTE: If AddRequestLobbyListDistanceFilter is not called, k_ELobbyDistanceFilterDefault will be used, which will only find matches in the same or nearby regions.
	 * NOTE: This will only return lobbies that are not full, and only lobbies that are k_ELobbyTypePublic or k_ELobbyTypeInvisible, and are set to joinable with SetLobbyJoinable.
	 * NOTE: This also returns as a callback for compatibility with older applications, but you should use the call result if possible.
	 *
	 * @return FSteamAPICall - SteamAPICall_t to be used with a LobbyMatchList_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall RequestLobbyList() const { return SteamMatchmaking()->RequestLobbyList(); }

	/**
	 * Broadcasts a chat (text or binary data) message to the all of the users in the lobby.
	 * All users in the lobby (including the local user) will receive a LobbyChatMsg_t callback with the message.
	 * If you're sending binary data, you should prefix a header to the message so that you know to treat it as your custom data rather than a plain old text message.
	 * For communication that needs to be arbitrated (for example having a user pick from a set of characters, and making sure only one user has picked a character), you can use the lobby owner as the decision maker. GetLobbyOwner returns the current lobby owner. There is guaranteed to always be one and only one lobby member who is the owner. So for the choose-a-character scenario, the user who is picking a character would send the binary message 'I want to be Zoe', the lobby owner would see that message, see if it was OK, and broadcast the appropriate result (user X is Zoe).
	 * These messages are sent via the Steam back-end, and so the bandwidth available is limited. For higher-volume traffic like voice or game data, you'll want to use the Steam Networking API.
	 * Triggers a LobbyChatMsg_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to send the chat message to.
	 * @param FString Message - This can be text or binary data, up to 4 Kilobytes in size.
	 * @return bool - true if the message was successfully sent. false if the message is too small or too large, or no connection to Steam could be made.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SendLobbyChatMsg(FSteamID SteamIDLobby, FString Message) const;

	/**
	 * Sets a key/value pair in the lobby metadata. This can be used to set the the lobby name, current map, game mode, etc.
	 * This can only be set by the owner of the lobby. Lobby members should use SetLobbyMemberData instead.
	 * Each user in the lobby will be receive notification of the lobby data change via a LobbyDataUpdate_t callback, and any new users joining will receive any existing data.
	 * This will only send the data if it has changed. There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to set the metadata for.
	 * @param const FString & Key - The key to set the data for. This can not be longer than k_nMaxLobbyKeyLength.
	 * @param const FString & Value - The value to set. This can not be longer than k_cubChatMetadataMax.
	 * @return bool - true if the data has been set successfully. false if steamIDLobby was invalid, or the key/value are too long.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyData(FSteamID SteamIDLobby, const FString& Key, const FString& Value) const { return SteamMatchmaking()->SetLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Sets the game server associated with the lobby.
	 * This can only be set by the owner of the lobby.
	 * Either the IP/Port or the Steam ID of the game server must be valid, depending on how you want the clients to be able to connect.
	 * A LobbyGameCreated_t callback will be sent to all players in the lobby, usually at this point, the users will join the specified game server.
	 * Triggers a LobbyGameCreated_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to set the game server information for.
	 * @param const FString & GameServerIP - Sets the IP address of the game server
	 * @param int32 GameServerPort - Sets the connection port of the game server, in host order.
	 * @param FSteamID SteamIDGameServer - Sets the Steam ID of the game server. Use k_steamIDNil if you're not setting this.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void SetLobbyGameServer(FSteamID SteamIDLobby, const FString& GameServerIP, int32 GameServerPort, FSteamID SteamIDGameServer) const;

	/**
	 * Sets whether or not a lobby is joinable by other players. This always defaults to enabled for a new lobby.
	 * If joining is disabled, then no players can join, even if they are a friend or have been invited.
	 * Lobbies with joining disabled will not be returned from a lobby search.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby
	 * @param bool bLobbyJoinable - Enable (true) or disable (false) allowing users to join this lobby?
	 * @return bool - true upon success; otherwise, false if you're not the owner of the lobby.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyJoinable(FSteamID SteamIDLobby, bool bLobbyJoinable) const { return SteamMatchmaking()->SetLobbyJoinable(SteamIDLobby, bLobbyJoinable); }

	/**
	 * Sets per-user metadata for the local user.
	 * Each user in the lobby will be receive notification of the lobby data change via a LobbyDataUpdate_t callback, and any new users joining will receive any existing data.
	 * There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.
	 * Triggers a LobbyDataUpdate_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to set our metadata in.
	 * @param const FString & Key - The key to set the data for. This can not be longer than k_nMaxLobbyKeyLength.
	 * @param const FString & Value - The value to set. This can not be longer than k_cubChatMetadataMax.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void SetLobbyMemberData(FSteamID SteamIDLobby, const FString& Key, const FString& Value) { SteamMatchmaking()->SetLobbyMemberData(SteamIDLobby, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Set the maximum number of players that can join the lobby.
	 * This is also set when you create the lobby with CreateLobby.
	 * This can only be set by the owner of the lobby.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to set the member limit for.
	 * @param uint8 MaxMembers - The maximum number of players allowed in this lobby. This can not be above 250.
	 * @return bool - true if the limit was successfully set. false if you are not the owner of the specified lobby.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyMemberLimit(FSteamID SteamIDLobby, uint8 MaxMembers = 5) const { return SteamMatchmaking()->SetLobbyMemberLimit(SteamIDLobby, MaxMembers); }

	/**
	 * Changes who the lobby owner is.
	 * This can only be set by the owner of the lobby. This will trigger a LobbyDataUpdate_t for all of the users in the lobby, each user should update their local state to reflect the new owner.
	 * This is typically accomplished by displaying a crown icon next to the owners name.
	 * Triggers a LobbyDataUpdate_t callback.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby where the owner change will take place.
	 * @param FSteamID SteamIDNewOwner - The Steam ID of the user that will be the new owner of the lobby, they must be in the lobby.
	 * @return bool - true if the owner was successfully changed. false if you're not the current owner of the lobby, steamIDNewOwner is not a member in the lobby, or if no connection to Steam could be made.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyOwner(FSteamID SteamIDLobby, FSteamID SteamIDNewOwner) const { return SteamMatchmaking()->SetLobbyOwner(SteamIDLobby, SteamIDNewOwner); }

	/**
	 * Updates what type of lobby this is.
	 * This is also set when you create the lobby with CreateLobby.
	 * This can only be set by the owner of the lobby.
	 *
	 * @param FSteamID SteamIDLobby - The Steam ID of the lobby to set the type of.
	 * @param ESteamLobbyType LobbyType - The new lobby type to that will be set.
	 * @return bool - true upon success; otherwise, false if you're not the owner of the lobby.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyType(FSteamID SteamIDLobby, ESteamLobbyType LobbyType) const { return SteamMatchmaking()->SetLobbyType(SteamIDLobby, (ELobbyType)LobbyType); }

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnFavoritesListAccountsUpdated"))
	FOnFavoritesListAccountsUpdatedDelegate OnFavoritesListAccountsUpdatedDelegate;

	/** A server was added/removed from the favorites list, you should refresh now. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnFavoritesListChanged"))
	FOnFavoritesListChangedDelegate OnFavoritesListChangedDelegate;

	/** A chat (text or binary) message for this lobby has been received. After getting this you must use GetLobbyChatEntry to retrieve the contents of this message. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyChatMsg"))
	FOnLobbyChatMsgDelegate OnLobbyChatMsgDelegate;

	/** A lobby chat room state has changed, this is usually sent when a user has joined or left the lobby. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyChatUpdate"))
	FOnLobbyChatUpdateDelegate OnLobbyChatUpdateDelegate;

	/** Result of our request to create a Lobby. At this point, the lobby has been joined and is ready for use, a LobbyEnter_t callback will also be received (since the local user is joining their own lobby). */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyCreated"))
	FOnLobbyCreatedDelegate OnLobbyCreatedDelegate;

	/**
	 * The lobby metadata has changed.
	 * If m_ulSteamIDMember is a user in the lobby, then use GetLobbyMemberData to access per-user details; otherwise, if m_ulSteamIDMember == m_ulSteamIDLobby, use GetLobbyData to access the lobby metadata.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyDataUpdate"))
	FOnLobbyDataUpdateDelegate OnLobbyDataUpdateDelegate;

	/** Recieved upon attempting to enter a lobby. Lobby metadata is available to use immediately after receiving this. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyEnter"))
	FOnLobbyEnterDelegate OnLobbyEnterDelegate;

	/**
	 * A game server has been set via SetLobbyGameServer for all of the members of the lobby to join.
	 * It's up to the individual clients to take action on this; the typical game behavior is to leave the lobby and connect to the specified game server; but the lobby may stay open throughout the session if desired.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyGameCreated"))
	FOnLobbyGameCreatedDelegate OnLobbyGameCreatedDelegate;

	/**
	 * Someone has invited you to join a Lobby. Normally you don't need to do anything with this, as the Steam UI will also display a '<user> has invited you to the lobby, join?' notification and message.
	 * If the user outside a game chooses to join, your game will be launched with the parameter +connect_lobby <64-bit lobby id>, or with the callback GameLobbyJoinRequested_t if they're already in-game.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyInvite"))
	FOnLobbyInviteDelegate OnLobbyInviteDelegate;

	// This is currently unused
	/*UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyKicked"))
	FOnLobbyKickedDelegate m_OnLobbyKicked;*/

	/** Result when requesting the lobby list. You should iterate over the returned lobbies with GetLobbyByIndex, from 0 to m_nLobbiesMatching-1. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyMatchList"))
	FOnLobbyMatchListDelegate OnLobbyMatchListDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnFavoritesListAccountsUpdated, FavoritesListAccountsUpdated_t, OnFavoritesListAccountsUpdatedCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnFavoritesListChanged, FavoritesListChanged_t, OnFavoritesListChangedCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyChatMsg, LobbyChatMsg_t, OnLobbyChatMsgCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyChatUpdate, LobbyChatUpdate_t, OnLobbyChatUpdateCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyCreated, LobbyCreated_t, OnLobbyCreatedCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyDataUpdate, LobbyDataUpdate_t, OnLobbyDataUpdateCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyEnter, LobbyEnter_t, OnLobbyEnterCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyGameCreated, LobbyGameCreated_t, OnLobbyGameCreatedCallback);
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyInvite, LobbyInvite_t, OnLobbyInviteCallback);
	// STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyKicked, LobbyKicked_t, OnLobbyKickedCallback); // This is currently unused
	STEAM_CALLBACK_MANUAL(USteamMatchmaking, OnLobbyMatchList, LobbyMatchList_t, OnLobbyMatchListCallback);
};

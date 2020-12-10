// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamMatchmaking.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFavoritesListAccountsUpdatedDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnFavoritesListChangedDelegate, FString, IP, int32, QueryPort, int32, ConnectionPort, int32, AppID, TArray<ESteamFavoriteFlags>, Flags, bool, bAdd, FAccountID, AccountID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLobbyChatMsgDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDUser, ESteamChatEntryType, ChatEntryType, int32, ChatID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLobbyChatUpdateDelegate, FSteamID, SteamIDLobby, FSteamID, SteamIDUserChanged, FSteamID, SteamIDMakingChange, TArray<ESteamChatMemberStateChange>, ChatMemberStateChange);
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

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	int32 AddFavoriteGame(int32 AppID, const FString& IP, int32 ConnPort, int32 QueryPort, const TArray<ESteamFavoriteFlags>& Flags, int32 TimeLastPlayedOnServer) const;

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListDistanceFilter(ESteamLobbyDistanceFilter LobbyDistanceFilter) { SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)LobbyDistanceFilter); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable) { SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(SlotsAvailable); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListNearValueFilter(const FString& KeyToMatch, int32 ValueToBeCloseTo) { SteamMatchmaking()->AddRequestLobbyListNearValueFilter(TCHAR_TO_UTF8(*KeyToMatch), ValueToBeCloseTo); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListNumericalFilter(const FString& KeyToMatch, int32 ValueToMatch, ESteamLobbyComparison ComparisonType) { SteamMatchmaking()->AddRequestLobbyListNumericalFilter(TCHAR_TO_UTF8(*KeyToMatch), ValueToMatch, (ELobbyComparison)((uint8)ComparisonType - 2)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListResultCountFilter(int32 MaxResults) { SteamMatchmaking()->AddRequestLobbyListResultCountFilter(MaxResults); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void AddRequestLobbyListStringFilter(const FString& KeyToMatch, const FString& ValueToMatch, ESteamLobbyComparison ComparisonType) { SteamMatchmaking()->AddRequestLobbyListStringFilter(TCHAR_TO_UTF8(*KeyToMatch), TCHAR_TO_UTF8(*ValueToMatch), (ELobbyComparison)((uint8)ComparisonType - 2)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall CreateLobby(ESteamLobbyType LobbyType = ESteamLobbyType::FriendsOnly, uint8 MaxMembers = 1) const { return SteamMatchmaking()->CreateLobby((ELobbyType)LobbyType, MaxMembers); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool DeleteLobbyData(FSteamID SteamIDLobby, const FString& Key) const { return SteamMatchmaking()->DeleteLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetFavoriteGame(int32 GameIndex, int32& AppID, FString& IP, int32& ConnPort, int32& QueryPort, TArray<ESteamFavoriteFlags>& Flags, int32& TimeLastPlayedOnServer) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetFavoriteGameCount() const { return SteamMatchmaking()->GetFavoriteGameCount(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyByIndex(int32 LobbyIndex) const { return SteamMatchmaking()->GetLobbyByIndex(LobbyIndex).ConvertToUint64(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyChatEntry(FSteamID SteamIDLobby, int32 ChatID, FSteamID& SteamIDUser, FString& Message, ESteamChatEntryType& ChatEntryType) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FString GetLobbyData(FSteamID SteamIDLobby, const FString& Key) const { return UTF8_TO_TCHAR(SteamMatchmaking()->GetLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key))); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetLobbyDataByIndex(FSteamID SteamIDLobby, int32 LobbyData, FString& Key, FString& Value) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyDataCount(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyDataCount(SteamIDLobby); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool GetLobbyGameServer(FSteamID SteamIDLobby, FString& GameServerIP, int32& GameServerPort, FSteamID& SteamIDGameServer) const;

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyMemberByIndex(FSteamID SteamIDLobby, int32 MemberIndex) const { return SteamMatchmaking()->GetLobbyMemberByIndex(SteamIDLobby, MemberIndex).ConvertToUint64(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FString GetLobbyMemberData(FSteamID SteamIDLobby, FSteamID SteamIDUser, const FString& Key) const { return UTF8_TO_TCHAR(SteamMatchmaking()->GetLobbyMemberData(SteamIDLobby, SteamIDUser, TCHAR_TO_UTF8(*Key))); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetLobbyMemberLimit(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyMemberLimit(SteamIDLobby); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	FSteamID GetLobbyOwner(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetLobbyOwner(SteamIDLobby).ConvertToUint64(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Matchmaking")
	int32 GetNumLobbyMembers(FSteamID SteamIDLobby) const { return SteamMatchmaking()->GetNumLobbyMembers(SteamIDLobby); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool InviteUserToLobby(FSteamID SteamIDLobby, FSteamID SteamIDInvitee) const { return SteamMatchmaking()->InviteUserToLobby(SteamIDLobby, SteamIDInvitee); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall JoinLobby(FSteamID SteamIDLobby) const { return SteamMatchmaking()->JoinLobby(SteamIDLobby); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void LeaveLobby(FSteamID SteamIDLobby) { SteamMatchmaking()->LeaveLobby(SteamIDLobby); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool RemoveFavoriteGame(int32 AppID, const FString& IP, int32 ConnPort, int32 QueryPort, const TArray<ESteamFavoriteFlags>& Flags) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool RequestLobbyData(FSteamID SteamIDLobby) const { return SteamMatchmaking()->RequestLobbyData(SteamIDLobby); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	FSteamAPICall RequestLobbyList() const { return SteamMatchmaking()->RequestLobbyList(); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SendLobbyChatMsg(FSteamID SteamIDLobby, FString Message) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyData(FSteamID SteamIDLobby, const FString& Key, const FString& Value) const { return SteamMatchmaking()->SetLobbyData(SteamIDLobby, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void SetLobbyGameServer(FSteamID SteamIDLobby, const FString& GameServerIP, int32 GameServerPort, FSteamID SteamIDGameServer) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyJoinable(FSteamID SteamIDLobby, bool bLobbyJoinable) const { return SteamMatchmaking()->SetLobbyJoinable(SteamIDLobby, bLobbyJoinable); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Matchmaking")
	void SetLobbyMemberData(FSteamID SteamIDLobby, const FString& Key, const FString& Value) { SteamMatchmaking()->SetLobbyMemberData(SteamIDLobby, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyMemberLimit(FSteamID SteamIDLobby, uint8 MaxMembers = 5) const { return SteamMatchmaking()->SetLobbyMemberLimit(SteamIDLobby, MaxMembers); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyOwner(FSteamID SteamIDLobby, FSteamID SteamIDNewOwner) const { return SteamMatchmaking()->SetLobbyOwner(SteamIDLobby, SteamIDNewOwner); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Matchmaking")
	bool SetLobbyType(FSteamID SteamIDLobby, ESteamLobbyType LobbyType) const { return SteamMatchmaking()->SetLobbyType(SteamIDLobby, (ELobbyType)LobbyType); }

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnFavoritesListAccountsUpdated"))
	FOnFavoritesListAccountsUpdatedDelegate m_OnFavoritesListAccountsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnFavoritesListChanged"))
	FOnFavoritesListChangedDelegate m_OnFavoritesListChanged;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyChatMsg"))
	FOnLobbyChatMsgDelegate m_OnLobbyChatMsg;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyChatUpdate"))
	FOnLobbyChatUpdateDelegate m_OnLobbyChatUpdate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyCreated"))
	FOnLobbyCreatedDelegate m_OnLobbyCreated;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyDataUpdate"))
	FOnLobbyDataUpdateDelegate m_OnLobbyDataUpdate;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyEnter"))
	FOnLobbyEnterDelegate m_OnLobbyEnter;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyGameCreated"))
	FOnLobbyGameCreatedDelegate m_OnLobbyGameCreated;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyInvite"))
	FOnLobbyInviteDelegate m_OnLobbyInvite;

	// This is currently unused
	/*UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyKicked"))
	FOnLobbyKickedDelegate m_OnLobbyKicked;*/

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Matchmaking", meta = (DisplayName = "OnLobbyMatchList"))
	FOnLobbyMatchListDelegate m_OnLobbyMatchList;

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

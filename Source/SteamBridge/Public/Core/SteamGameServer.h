// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamGameServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssociateWithClanResultDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnComputeNewPlayerCompatibilityResultDelegate, ESteamResult, Result, int32, PlayersThatDontLikeCandidate, int32, PlayersThatDoesntLikeCandidate, int32, ClanPlayersThatDontLikeCandidate, FSteamID, SteamIDCandidate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGSClientApproveDelegate, FSteamID, SteamID, FSteamID, OwnerSteamID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGSClientDenyDelegate, FSteamID, SteamID, ESteamDenyReason, DenyReason, FString, OptionalText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGSClientGroupStatusDelegate, FSteamID, SteamIDUser, FSteamID, SteamIDGroup, bool, bMember, bool, bOfficer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGSClientKickDelegate, FSteamID, SteamID, ESteamDenyReason, DenyReason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSPolicyResponseDelegate, bool, bSecure);

/**
 * Provides the core of the Steam Game Servers API.
 * https://partner.steamgames.com/doc/api/ISteamGameServer
 */
UCLASS()
class STEAMBRIDGE_API USteamGameServer final : public UObject
{
	GENERATED_BODY()

public:
	USteamGameServer();
	~USteamGameServer();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Game Server", CompactNodeTitle = "SteamGameServer"))
	static USteamGameServer* GetSteamGameServer() { return USteamGameServer::StaticClass()->GetDefaultObject<USteamGameServer>(); }

	/**
	 * Associate this game server with this clan for the purposes of computing player compatibility.
	 *
	 * @param FSteamID SteamIDClan - The Steam ID of the group you want to be associated with.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a AssociateWithClanResult_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	FSteamAPICall AssociateWithClan(const FSteamID SteamIDClan) const { return SteamGameServer()->AssociateWithClan(SteamIDClan); }

	/**
	 * Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
	 * The ticket is created on the entity with ISteamUser::GetAuthSessionTicket or GetAuthSessionTicket and then needs to be provided over the network for the other end to validate.
	 * This registers for ValidateAuthTicketResponse_t callbacks if the entity goes offline or cancels the ticket. See EAuthSessionResponse for more information.
	 * When the multiplayer session terminates you must call EndAuthSession.
	 *
	 * @param TArray<uint8> AuthTicket - The auth ticket to validate.
	 * @param FSteamID SteamID - The entity's Steam ID that sent this ticket.
	 * @return ESteamBeginAuthSessionResult
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	ESteamBeginAuthSessionResult BeginAuthSession(TArray<uint8> AuthTicket, const FSteamID SteamID) const;

	/**
	 * Checks if the game server is logged on.
	 *
	 * @return bool - true if the game server is logged on; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	bool BLoggedOn() const { return SteamGameServer()->BLoggedOn(); }

	/**
	 * Checks whether the game server is in "Secure" mode.
	 *
	 * @return bool - true if the game server secure; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	bool BSecure() const { return SteamGameServer()->BSecure(); }

	/**
	 * Cancels an auth ticket received from ISteamUser::GetAuthSessionTicket. This should be called when no longer playing with the specified entity.
	 *
	 * @param FHAuthTicket AuthTicketHandle - The active auth ticket to cancel.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void CancelAuthTicket(const FHAuthTicket AuthTicketHandle) { SteamGameServer()->CancelAuthTicket(AuthTicketHandle); }

	/**
	 * Clears the whole list of key/values that are sent in rules queries.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void ClearAllKeyValues() { SteamGameServer()->ClearAllKeyValues(); }

	// #TODO ComputeNewPlayerCompatibility

	/**
	 * Tells the Steam master servers whether or not you want to be active.
	 * If this is enabled then the server will talk to the master servers, if it's not then incoming messages are ignored and heartbeats will not be sent.
	 *
	 * @param bool bActive - Enable (true) or disable (false) the master server updater.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void EnableHeartbeats(const bool bActive) { SteamGameServer()->EnableHeartbeats(bActive); }

	/**
	 * Ends an auth session that was started with BeginAuthSession. This should be called when no longer playing with the specified entity.
	 *
	 * @param FSteamID SteamID - The entity to end the active auth session with.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void EndAuthSession(const FSteamID SteamID) { SteamGameServer()->EndAuthSession(SteamID); }

	/**
	 * Force a heartbeat to the Steam master servers at the next opportunity.
	 * You usually don't need to use this.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void ForceHeartbeat() { SteamGameServer()->ForceHeartbeat(); }

	/**
	 * Retrieve a authentication ticket to be sent to the entity who wishes to authenticate you.
	 * After calling this you can send the ticket to the entity where they can then call ISteamUser::BeginAuthSession to verify this entities integrity.
	 * When creating a ticket for use by the ISteamUserAuth/AuthenticateUserTicket Web API, the calling application should wait for the GetAuthSessionTicketResponse_t callback generated by the API -
	 * call before attempting to use the ticket to ensure that the ticket has been communicated to the server. If this callback does not come in a timely fashion (10 - 20 seconds), then your -
	 * client is not connected to Steam, and the AuthenticateUserTicket call will fail because it can not authenticate the user.
	 * Triggers a GetAuthSessionTicketResponse_t callback.
	 *
	 * @param TArray<uint8> & AuthTicket - 	The buffer where the new auth ticket will be copied into if the call was successful.
	 * @return FHAuthTicket - A handle to the auth ticket. When you're done interacting with the entity you must call CancelAuthTicket on the handle.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	FHAuthTicket GetAuthSessionTicket(TArray<uint8>& AuthTicket) const;

	// #TODO GetNextOutgoingPacket


	// #NOTE This method only returns IPv4 for now, will eventually return a struct of the IPs (to support IPv6)
	/**
	 * Gets the public IP of the server according to Steam.
	 * This is useful when the server is behind NAT and you want to advertise its IP in a lobby for other clients to directly connect to.
	 *
	 * @return FString - Returns the IP address (IPv4) as an FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	FString GetPublicIP() const;

	/**
	 * Gets the Steam ID of the game server.
	 *
	 * @return FSteamID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	FSteamID GetSteamID() const { return SteamGameServer()->GetSteamID().ConvertToUint64(); }

	// #TODO HandleIncomingPacket
	// #TODO InitGameServer

	/**
	 * Begin process of logging the game server out of steam.
	 * Triggers a SteamServerConnectFailure_t callback.
	 * Triggers a SteamServersConnected_t callback.
	 * Triggers a SteamServersDisconnected_t callback.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void LogOff() { SteamGameServer()->LogOff(); }

	/**
	 * Begin process to login to a persistent game server account.
	 * Triggers a SteamServerConnectFailure_t callback.
	 * Triggers a SteamServersConnected_t callback.
	 * Triggers a SteamServersDisconnected_t callback.
	 *
	 * @param const FString & Token
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void LogOn(const FString& Token) { SteamGameServer()->LogOn(TCHAR_TO_UTF8(*Token)); }

	/**
	 * Login to a generic, anonymous account.
	 * Triggers a SteamServerConnectFailure_t callback.
	 * Triggers a SteamServersConnected_t callback.
	 * Triggers a SteamServersDisconnected_t callback.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void LogOnAnonymous() { SteamGameServer()->LogOnAnonymous(); }

	/**
	 * Checks if a user is in the specified Steam group.
	 * Triggers a GSClientGroupStatus_t callback.
	 *
	 * @param FSteamID SteamIDUser - The user to check the group status of.
	 * @param FSteamID SteamIDGroup - The group to check.
	 * @return bool - true if the call was successfully sent out to the Steam servers; otherwise, false if we're not connected to the steam servers or an invalid user or group was provided.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|GameServer")
	bool RequestUserGroupStatus(const FSteamID SteamIDUser, const FSteamID SteamIDGroup) const { return SteamGameServer()->RequestUserGroupStatus(SteamIDUser, SteamIDGroup); }

	/**
	 * Sets the number of bot/AI players on the game server. The default value is 0.
	 *
	 * @param int32 BotPlayers - The number of bot/AI players currently playing on the server.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetBotPlayerCount(const int32 BotPlayers) { SteamGameServer()->SetBotPlayerCount(BotPlayers); }

	/**
	 * Sets the whether this is a dedicated server or a listen server. The default is listen server.
	 * NOTE: This only be set before calling LogOn or LogOnAnonymous.
	 *
	 * @param bool bDedicated - Is this a dedicated server (true) or a listen server (false)?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetDedicatedServer(const bool bDedicated) { SteamGameServer()->SetDedicatedServer(bDedicated); }

	/**
	 * Sets a string defining the "gamedata" for this server, this is optional, but if set it allows users to filter in the matchmaking/server-browser interfaces based on the value.
	 * This is usually formatted as a comma or semicolon separated list.
	 * Don't set this unless it actually changes, its only uploaded to the master once; when acknowledged.
	 *
	 * @param const FString & GameData - The new "gamedata" value to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerGameData.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetGameData(const FString& GameData) { SteamGameServer()->SetGameData(TCHAR_TO_UTF8(*GameData)); }

	/**
	 * Sets the game description. Setting this to the full name of your game is recommended.
	 * NOTE: This is required for all game servers and can only be set before calling LogOn or LogOnAnonymous.
	 *
	 * @param const FString GameDescription - The description of your game. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerGameDescription.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetGameDescription(const FString GameDescription) { SteamGameServer()->SetGameDescription(TCHAR_TO_UTF8(*GameDescription)); }

	/**
	 * Sets a string defining the "gametags" for this server, this is optional, but if set it allows users to filter in the matchmaking/server-browser interfaces based on the value.
	 * This is usually formatted as a comma or semicolon separated list.
	 * Don't set this unless it actually changes, its only uploaded to the master once; when acknowledged.
	 *
	 * @param const FString & GameTags - The new "gametags" value to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerTags.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetGameTags(const FString& GameTags) { SteamGameServer()->SetGameTags(TCHAR_TO_UTF8(*GameTags)); }

	/**
	 * Changes how often heartbeats are sent to the Steam master servers.
	 * You usually don't need to use this.
	 *
	 * @param int32 HeartbeatInterval - The interval between sending heartbeats in milliseconds. Typically would range between 250-1000. Use -1 to use the default value.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetHeartbeatInterval(const int32 HeartbeatInterval) { SteamGameServer()->SetHeartbeatInterval(HeartbeatInterval); }

	/**
	 * Add/update a rules key/value pair.
	 *
	 * @param const FString & Key
	 * @param const FString & Value
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetKeyValue(const FString& Key, const FString& Value) { SteamGameServer()->SetKeyValue(TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Sets the name of map to report in the server browser.
	 *
	 * @param const FString & MapName - The new map name to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerMapName.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetMapName(const FString& MapName) { SteamGameServer()->SetMapName(TCHAR_TO_UTF8(*MapName)); }

	/**
	 * Sets the maximum number of players allowed on the server at once.
	 * This value may be changed at any time.
	 *
	 * @param int32 PlayersMax - The new maximum number of players allowed on this server.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetMaxPlayerCount(const int32 PlayersMax) { SteamGameServer()->SetMaxPlayerCount(PlayersMax); }

	/**
	 * Sets the game directory.
	 * This should be the same directory game where gets installed into. Just the folder name, not the whole path. I.e. "Spacewar".
	 * NOTE: This is required for all game servers and can only be set before calling LogOn or LogOnAnonymous.
	 *
	 * @param const FString & ModDir - The game directory to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerGameDir
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetModDir(const FString& ModDir) { SteamGameServer()->SetModDir(TCHAR_TO_UTF8(*ModDir)); }

	/**
	 * Set whether the game server will require a password once when the user tries to join.
	 *
	 * @param bool bPasswordProtected - Enable (true) or disable (false) password protection.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetPasswordProtected(const bool bPasswordProtected) { SteamGameServer()->SetPasswordProtected(bPasswordProtected); }

	/**
	 * Sets the game product identifier. This is currently used by the master server for version checking purposes.
	 * Converting the games app ID to a string for this is recommended.
	 * NOTE: This is required for all game servers and can only be set before calling LogOn or LogOnAnonymous.
	 *
	 * @param const FString & Product - The unique identifier for your game. Must not be NULL or an empty string ("").
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetProduct(const FString& Product) { SteamGameServer()->SetProduct(TCHAR_TO_UTF8(*Product)); }

	/**
	 * Region identifier. This is an optional field, the default value is an empty string, meaning the "world" region.
	 *
	 * @param const FString & Region
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetRegion(const FString& Region) { SteamGameServer()->SetRegion(TCHAR_TO_UTF8(*Region)); }

	/**
	 * Sets the name of server as it will appear in the server browser.
	 *
	 * @param const FString & ServerName - The new server name to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerName.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetServerName(const FString& ServerName) { SteamGameServer()->SetServerName(TCHAR_TO_UTF8(*ServerName)); }

	/**
	 * Set whether the game server allows spectators, and what port they should connect on. The default value is 0, meaning the service is not used.
	 *
	 * @param int32 SpectatorPort - The port for spectators to join.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetSpectatorPort(const int32 SpectatorPort) { SteamGameServer()->SetSpectatorPort(FMath::Clamp<uint16>(SpectatorPort, 0, 65535)); }

	/**
	 * Sets the name of the spectator server. This is only used if spectator port is nonzero.
	 *
	 * @param const FString & SpectatorServerName - The spectator server name to set. Must not be NULL or an empty string (""). This can not be longer than k_cbMaxGameServerMapName.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	void SetSpectatorServerName(const FString& SpectatorServerName) { SteamGameServer()->SetSpectatorServerName(TCHAR_TO_UTF8(*SpectatorServerName)); }

	/**
	 * Checks if the user owns a specific piece of Downloadable Content (DLC).
	 * This can only be called after sending the users auth ticket to BeginAuthSession/
	 *
	 * @param FSteamID SteamID - The Steam ID of the user that sent the auth ticket.
	 * @param int32 AppID - The DLC App ID to check if the user owns it.
	 * @return ESteamUserHasLicenseForAppResult
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|GameServer")
	ESteamUserHasLicenseForAppResult UserHasLicenseForApp(const FSteamID SteamID, const int32 AppID) const { return (ESteamUserHasLicenseForAppResult)SteamGameServer()->UserHasLicenseForApp(SteamID, AppID); }

	/**
	 * Checks if the master server has alerted us that we are out of date.
	 * This reverts back to false after calling this function.
	 *
	 * @return bool - true if the master server wants this game server to update and restart; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|GameServer")
	bool WasRestartRequested() const { return SteamGameServer()->WasRestartRequested(); }

	/** Delegates */

	/** Sent when the game server attempted to be associated with a Steam Group. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnAssociateWithClanResult"))
	FOnAssociateWithClanResultDelegate OnAssociateWithClanResultDelegate;

	/** Sent when the game server is checking if the new player is a good fit for the server based on the frenemy system. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnComputeNewPlayerCompatibilityResult"))
	FOnComputeNewPlayerCompatibilityResultDelegate OnComputeNewPlayerCompatibilityResultDelegate;

	/** A client has been approved to connect to this game server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnGSClientApprove"))
	FOnGSClientApproveDelegate OnGSClientApproveDelegate;

	/** Called when a user has been denied to connection to this game server. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnGSClientDeny"))
	FOnGSClientDenyDelegate OnGSClientDenyDelegate;

	/** Called when we have received the group status of a user. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnGSClientGroupStatus"))
	FOnGSClientGroupStatusDelegate OnGSClientGroupStatusDelegate;

	/** Called when the game server should kick the user. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnGSClientKick"))
	FOnGSClientKickDelegate OnGSClientKickDelegate;

	/** Received when the game server requests to be displayed as secure (VAC protected) */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|GameServer", meta = (DisplayName = "OnGSPolicyResponse"))
	FOnGSPolicyResponseDelegate OnGSPolicyResponseDelegate;

protected:
private:
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnAssociateWithClanResult, AssociateWithClanResult_t, OnAssociateWithClanResultCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnComputeNewPlayerCompatibilityResult, ComputeNewPlayerCompatibilityResult_t, OnComputeNewPlayerCompatibilityResultCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnGSClientApprove, GSClientApprove_t, OnGSClientApproveCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnGSClientDeny, GSClientDeny_t, OnGSClientDenyCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnGSClientGroupStatus, GSClientGroupStatus_t, OnGSClientGroupStatusCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnGSClientKick, GSClientKick_t, OnGSClientKickCallback);
	STEAM_GAMESERVER_CALLBACK_MANUAL(USteamGameServer, OnGSPolicyResponse, GSPolicyResponse_t, OnGSPolicyResponseCallback);
};

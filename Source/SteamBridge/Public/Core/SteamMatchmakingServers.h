// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamMatchmakingServers.generated.h"

/**
 * Functions which provide access to the game server browser.
 * https://partner.steamgames.com/doc/api/ISteamMatchmakingServers
 */
UCLASS()
class STEAMBRIDGE_API USteamMatchmakingServers final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Matchmaking Servers", CompactNodeTitle = "SteamMatchmakingServers"))
	static USteamMatchmakingServers* GetSteamMatchmakingServers() { return USteamMatchmakingServers::StaticClass()->GetDefaultObject<USteamMatchmakingServers>(); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|MatchmakingServers")
	void CancelQuery(const FHServerListRequest& Request) { SteamMatchmakingServers()->CancelQuery(Request.Value); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|MatchmakingServers")
	void CancelServerQuery(FHServerQuery ServerQuery) { SteamMatchmakingServers()->CancelServerQuery(ServerQuery.Value); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MatchmakingServers")
	int32 GetServerCount(const FHServerListRequest& Request) const { return SteamMatchmakingServers()->GetServerCount(Request.Value); }

	// #TODO: GetServerDetails

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|MatchmakingServers")
	bool IsRefreshing(const FHServerListRequest& Request) const { return SteamMatchmakingServers()->IsRefreshing(Request.Value); }

	// #TODO: PingServer
	// #TODO: PlayerDetails

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|MatchmakingServers")
	void RefreshQuery(const FHServerListRequest& Request) { SteamMatchmakingServers()->RefreshQuery(Request.Value); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|MatchmakingServers")
	void RefreshServer(const FHServerListRequest& Request, int32 ServerIndex) { SteamMatchmakingServers()->RefreshServer(Request.Value, ServerIndex); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|MatchmakingServers")
	void ReleaseRequest(const FHServerListRequest& ServerListRequest) { SteamMatchmakingServers()->ReleaseRequest(ServerListRequest.Value); }

	// #TODO: RequestFavoritesServerList
	// #TODO: RequestFriendsServerList
	// #TODO: RequestHistoryServerList
	// #TODO: RequestInternetServerList
	// #TODO: RequestLANServerList
	// #TODO: RequestSpectatorServerList
	// #TODO: ServerRules


protected:
private:
};

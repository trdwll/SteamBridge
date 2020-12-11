// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamParties.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnJoinPartyDelegate, ESteamResult, Result, FPartyBeaconID, BeaconID, FSteamID, SteamIDBeaconOwner, FString, ConnectString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateBeaconDelegate, ESteamResult, Result, FPartyBeaconID, BeaconID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReservationNotificationDelegate, FPartyBeaconID, BeaconID, FSteamID, SteamIDJoiner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeNumOpenSlotsDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAvailableBeaconLocationsUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveBeaconsUpdatedDelegate);


/**
 * This API can be used to selectively advertise your multiplayer game session in a Steam chat room group. Tell Steam the number of player spots that are available for your party, and a join-game string, and it will
 * show a beacon in the selected group and allow that many users to “follow” the beacon to your party. Adjust the number of open slots if other players join through alternate matchmaking methods.
 * For example, you can use ISteamParties in conjunction with a private lobby. Create a private lobby, and then use ISteamParties::CreateBeacon to create a party "beacon" for the number of players desired. The game
 * connect string should indicate the ID of the private lobby.
 * The beacon will appear in Steam in the specified location (e.g. a Chat Room Group), and also via the in-game ISteamParties API as described below. Steam creates "reservation" slots for the number of desired players.
 * Whenever a user follows the beacon, Steam will hold a reservation slot for them and launch the game using the given connect string.
 * The game session that created the beacon will be notified of this reservation, so the game can display the appropriate "User <username> is joining your party" or some other indicator. Once the user joins successfully,
 * the game session should call ISteamParties::OnReservationCompleted to tell Steam that the user successfully joined (otherwise, Steam will eventually timeout their reservation and re-open the slot).
 * When all of the beacon slots are occupied - either by reservations for users still launching the game, or completed slots for users in the party - Steam will hide and disable the beacon.
 * To cancel the beacon - for instance when the party is full and the game begins - call ISteamParties::DestroyBeacon.
 * The client side of this operation - seeing and following beacons - can also be managed by your game. Using ISteamParties::GetNumActiveBeacons and ISteamParties::GetBeaconDetails, your game can get a list of beacons
 * from other users that are currently active in locations relevant to the current user. If the user desires, call ISteamParties::JoinParty to "follow" one of those beacons.
 * https://partner.steamgames.com/doc/api/ISteamParties
 */
UCLASS()
class STEAMBRIDGE_API USteamParties final : public UObject
{
	GENERATED_BODY()

public:
	USteamParties();
	~USteamParties();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Parties", CompactNodeTitle = "SteamParties"))
	static USteamParties* GetSteamParties() { return USteamParties::StaticClass()->GetDefaultObject<USteamParties>(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Parties")
	bool GetNumAvailableBeaconLocations(int32& NumLocations) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetAvailableBeaconLocations(TArray<FSteamPartyBeaconLocation>& LocationList) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall CreateBeacon(int32 OpenSlots, FSteamPartyBeaconLocation& BeaconLocation, const FString& ConnectString, const FString& Metadata) const;

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Parties")
	void OnReservationCompleted(FPartyBeaconID BeaconID, FSteamID SteamIDUser) { SteamParties()->OnReservationCompleted(BeaconID, SteamIDUser); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall ChangeNumOpenSlots(FPartyBeaconID BeaconID, int32 OpenSlots) const { return SteamParties()->ChangeNumOpenSlots(BeaconID, OpenSlots); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool DestroyBeacon(FPartyBeaconID BeaconID) const { return SteamParties()->DestroyBeacon(BeaconID); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	int32 GetNumActiveBeacons() const { return SteamParties()->GetNumActiveBeacons(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Parties")
	FPartyBeaconID GetBeaconByIndex(int32 Index) const { return SteamParties()->GetBeaconByIndex(Index); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetBeaconDetails(FPartyBeaconID BeaconID, FSteamID& SteamIDBeaconOwner, FSteamPartyBeaconLocation& BeaconLocation, FString& Metadata) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall JoinParty(FPartyBeaconID BeaconID) const { return SteamParties()->JoinParty(BeaconID); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetBeaconLocationData(FSteamPartyBeaconLocation BeaconLocation, ESteamPartyBeaconLocationData_ LocationData, FString& DataString) const;


	/** Delegates */


	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnJoinParty"))
	FOnJoinPartyDelegate m_OnJoinParty;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnCreateBeacon"))
	FOnCreateBeaconDelegate m_OnCreateBeacon;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnReservationNotification"))
	FOnReservationNotificationDelegate m_OnReservationNotification;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnChangeNumOpenSlots"))
	FOnChangeNumOpenSlotsDelegate m_OnChangeNumOpenSlots;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnAvailableBeaconLocationsUpdated"))
	FOnAvailableBeaconLocationsUpdatedDelegate m_OnAvailableBeaconLocationsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnActiveBeaconsUpdated"))
	FOnActiveBeaconsUpdatedDelegate m_OnActiveBeaconsUpdated;




protected:
private:
	STEAM_CALLBACK_MANUAL(USteamParties, OnJoinParty, JoinPartyCallback_t, OnJoinPartyCallback);
	STEAM_CALLBACK_MANUAL(USteamParties, OnCreateBeacon, CreateBeaconCallback_t, OnCreateBeaconCallback);
	STEAM_CALLBACK_MANUAL(USteamParties, OnReservationNotification, ReservationNotificationCallback_t, OnReservationNotificationCallback);
	STEAM_CALLBACK_MANUAL(USteamParties, OnChangeNumOpenSlots, ChangeNumOpenSlotsCallback_t, OnChangeNumOpenSlotsCallback);
	STEAM_CALLBACK_MANUAL(USteamParties, OnAvailableBeaconLocationsUpdated, AvailableBeaconLocationsUpdated_t, OnAvailableBeaconLocationsUpdatedCallback);
	STEAM_CALLBACK_MANUAL(USteamParties, OnActiveBeaconsUpdated, ActiveBeaconsUpdated_t, OnActiveBeaconsUpdatedCallback);
};

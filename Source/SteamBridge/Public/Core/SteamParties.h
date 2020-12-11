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

	/**
	 * Get the number of locations in which you are able to post a party beacon.
	 * Use this to size your result list for a call to ISteamParties::GetAvailableBeaconLocations.
	 *
	 * @param int32 & NumLocations
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Parties")
	bool GetNumAvailableBeaconLocations(int32& NumLocations) const;

	/**
	 * Get the list of locations in which you can post a party beacon.
	 *
	 * @param TArray<FSteamPartyBeaconLocation> & LocationList - Output list of available beacon locations.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetAvailableBeaconLocations(TArray<FSteamPartyBeaconLocation>& LocationList) const;

	/**
	 * Create a beacon. You can only create one beacon at a time. Steam will display the beacon in the specified location, and let up to unOpenSlots users "follow" the beacon to your party.
	 * If users join your party through other matchmaking, adjust the number of remaining open slots using ISteamParties::ChangeNumOpenSlots.
	 *
	 * @param int32 OpenSlots - Number of reservation slots to create for the beacon. Normally, this is the size of your desired party minus one (for the current user).
	 * @param FSteamPartyBeaconLocation & BeaconLocation - Location information for the beacon. Should be one of the locations returned by ISteamParties::GetAvailableBeaconLocations.
	 * @param const FString & ConnectString - Connect string that will be given to the game on launch for a user that follows the beacon.
	 * @param const FString & Metadata - 	Additional game metadata that can be set on the beacon, and is exposed via ISteamParties::GetBeaconDetails.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a CreateBeaconCallback_t call result. Returns k_uAPICallInvalid if the process already has an active beacon, or if the location information is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall CreateBeacon(int32 OpenSlots, FSteamPartyBeaconLocation& BeaconLocation, const FString& ConnectString, const FString& Metadata) const;

	/**
	 * When a user follows your beacon, Steam will reserve one of the open party slots for them, and send your game a ReservationNotificationCallback_t callback. When that user joins your party, call
	 * OnReservationCompleted to notify Steam that the user has joined successfully.
	 *
	 * @param FPartyBeaconID BeaconID - Beacon ID for the beacon created by your process.
	 * @param FSteamID SteamIDUser - 	SteamID of the user joining your party.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Parties")
	void OnReservationCompleted(FPartyBeaconID BeaconID, FSteamID SteamIDUser) { SteamParties()->OnReservationCompleted(BeaconID, SteamIDUser); }

	/**
	 * If a user joins your party through other matchmaking (perhaps a direct Steam friend, or your own matchmaking system), your game should reduce the number of open slots that Steam is managing through the party beacon.
	 * For example, if you created a beacon with five slots, and Steam sent you two ReservationNotificationCallback_t callbacks, and then a third user joined directly, you would want to call ChangeNumOpenSlots with a
	 * value of 2 for unOpenSlots. That value represents the total number of new users that you would like Steam to send to your party.
	 *
	 * @param FPartyBeaconID BeaconID - Beacon ID for the beacon created by your process.
	 * @param int32 OpenSlots - The new number of open slots in your party.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a ChangeNumOpenSlotsCallback_t call result. Returns k_uAPICallInvalid if the beacon ID is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall ChangeNumOpenSlots(FPartyBeaconID BeaconID, int32 OpenSlots) const { return SteamParties()->ChangeNumOpenSlots(BeaconID, OpenSlots); }

	/**
	 * Call this method to destroy the Steam party beacon. This will immediately cause Steam to stop showing the beacon in the target location. Note that any users currently in-flight may still arrive at your party expecting to join.
	 * Your game should call this method when either the party has been filled and the game is beginning, or the user has decided to abandon creating a party. The beacon will be destroyed automatically when your game exits,
	 * but the preferred behavior is for the game to call DestroyBeacon at the right time.
	 *
	 * @param FPartyBeaconID BeaconID - Beacon ID to be destroyed.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool DestroyBeacon(FPartyBeaconID BeaconID) const { return SteamParties()->DestroyBeacon(BeaconID); }

	/**
	 * Get the number of active party beacons created by other users for your game, that are visible to the current user.
	 *
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	int32 GetNumActiveBeacons() const { return SteamParties()->GetNumActiveBeacons(); }

	/**
	 * Use with ISteamParties::GetNumActiveBeacons to iterate the active beacons visible to the current user. unIndex is a zero-based index, so iterate over the range [0, GetNumActiveBeacons() - 1]. The return is a
	 * PartyBeaconID_t that can be used with ISteamParties::GetBeaconDetails to get information about the beacons suitable for display to the user.
	 *
	 * @param int32 Index - Index of Beacon.
	 * @return FPartyBeaconID - DESCHERE
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Parties")
	FPartyBeaconID GetBeaconByIndex(int32 Index) const { return SteamParties()->GetBeaconByIndex(Index); }

	/**
	 * Get details about the specified beacon. You can use the ISteamFriends API to get further details about pSteamIDBeaconOwner, and ISteamParties::GetBeaconLocationData to get further details about pLocation.
	 * The pchMetadata contents are specific to your game, and will be whatever was set (if anything) by the game process that created the beacon.
	 *
	 * @param FPartyBeaconID BeaconID - Beacon ID to query.
	 * @param FSteamID & SteamIDBeaconOwner - Creator of the beacon.
	 * @param FSteamPartyBeaconLocation & BeaconLocation - 	Location the beacon has been posted.
	 * @param FString & Metadata - Buffer to receive any additional metadata the game has set on this beacon (e.g. game mode). Will be NULL terminated on success
	 * @return bool - DESCHERE
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetBeaconDetails(FPartyBeaconID BeaconID, FSteamID& SteamIDBeaconOwner, FSteamPartyBeaconLocation& BeaconLocation, FString& Metadata) const;

	/**
	 * When the user indicates they wish to join the party advertised by a given beacon, call this method. On success, Steam will reserve a slot for this user in the party and return the necessary "join game" string to use to complete the connection.
	 *
	 * @param FPartyBeaconID BeaconID - Beacon ID for the party you wish to join.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a JoinParty_t call result. Returns k_uAPICallInvalid if the beacon ID is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	FSteamAPICall JoinParty(FPartyBeaconID BeaconID) const { return SteamParties()->JoinParty(BeaconID); }

	/**
	 * Query general metadata for the given beacon location. For instance the Name, or the URL for an icon if the location type supports icons (for example, the icon for a Steam Chat Room Group).
	 *
	 * @param FSteamPartyBeaconLocation BeaconLocation - Location to query.
	 * @param ESteamPartyBeaconLocationData_ LocationData - Type of location data you wish to get.
	 * @param FString & DataString - 	Output buffer for location data string. Will be NULL-terminated on success.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Parties")
	bool GetBeaconLocationData(FSteamPartyBeaconLocation BeaconLocation, ESteamPartyBeaconLocationData_ LocationData, FString& DataString) const;

	/** Delegates */

	/** This callback is used as a call response for ISteamParties::JoinParty. On success, you will have reserved a slot in the beacon-owner's party, and should use m_rgchConnectString to connect to their game and complete the process. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnJoinParty"))
	FOnJoinPartyDelegate m_OnJoinParty;

	/**
	 * This callback is used as a call response for ISteamParties::CreateBeacon. If successful, your beacon has been posted in the desired location and you may start receiving ISteamParties::ReservationNotificationCallback_t
	 * callbacks for users following the beacon.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnCreateBeacon"))
	FOnCreateBeaconDelegate m_OnCreateBeacon;

	/** After creating a beacon, when a user "follows" that beacon Steam will send you this callback to know that you should be prepared for the user to join your game. When they do join, be sure to call ISteamParties::OnReservationCompleted to let Steam know. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnReservationNotification"))
	FOnReservationNotificationDelegate m_OnReservationNotification;

	/** Call result for ISteamParties::ChangeNumOpenSlots.  */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnChangeNumOpenSlots"))
	FOnChangeNumOpenSlotsDelegate m_OnChangeNumOpenSlots;

	/** Notification that the list of available locations for posting a beacon has been updated.  */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Parties", meta = (DisplayName = "OnAvailableBeaconLocationsUpdated"))
	FOnAvailableBeaconLocationsUpdatedDelegate m_OnAvailableBeaconLocationsUpdated;

	/** Notification that the list of active beacons visible to the current user has changed.  */
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

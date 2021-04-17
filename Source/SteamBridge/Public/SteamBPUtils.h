// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamBPUtils.generated.h"

/**
 * A utility class for BP. (implements things like != and == nodes)
 */
UCLASS()
class STEAMBRIDGE_API USteamBPUtils final : public UObject
{
	GENERATED_BODY()

public:
	/** Operators */

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamID_Equal(FSteamID SteamIDA, FSteamID SteamIDB) { return SteamIDA == SteamIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamID_NotEqual(FSteamID SteamIDA, FSteamID SteamIDB) { return SteamIDA != SteamIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamAPICall_Equal(FSteamAPICall SteamAPICallA, FSteamAPICall SteamAPICallB) { return SteamAPICallA == SteamAPICallB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamAPICall_NotEqual(FSteamAPICall SteamAPICallA, FSteamAPICall SteamAPICallB) { return SteamAPICallA != SteamAPICallB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamItemInstanceID_Equal(FSteamItemInstanceID SteamItemInstanceIDA, FSteamItemInstanceID SteamItemInstanceIDB) { return SteamItemInstanceIDA == SteamItemInstanceIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamItemInstanceID_NotEqual(FSteamItemInstanceID SteamItemInstanceIDA, FSteamItemInstanceID SteamItemInstanceIDB) { return SteamItemInstanceIDA != SteamItemInstanceIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamInventoryUpdateHandle_Equal(FSteamInventoryUpdateHandle SteamInventoryUpdateHandleA, FSteamInventoryUpdateHandle SteamInventoryUpdateHandleB) { return SteamInventoryUpdateHandleA == SteamInventoryUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamInventoryUpdateHandle_NotEqual(FSteamInventoryUpdateHandle SteamInventoryUpdateHandleA, FSteamInventoryUpdateHandle SteamInventoryUpdateHandleB) { return SteamInventoryUpdateHandleA != SteamInventoryUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HAuthTicket_Equal(FHAuthTicket HAuthTicketA, FHAuthTicket HAuthTicketB) { return HAuthTicketA == HAuthTicketB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HAuthTicket_NotEqual(FHAuthTicket HAuthTicketA, FHAuthTicket HAuthTicketB) { return HAuthTicketA != HAuthTicketB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HSteamUser_Equal(FHSteamUser HSteamUserA, FHSteamUser HSteamUserB) { return HSteamUserA == HSteamUserB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HSteamUser_NotEqual(FHSteamUser HSteamUserA, FHSteamUser HSteamUserB) { return HSteamUserA != HSteamUserB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool AppID_Equal(FAppID AppIDA, FAppID AppIDB) { return AppIDA == AppIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool AppID_NotEqual(FAppID AppIDA, FAppID AppIDB) { return AppIDA != AppIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool AccountID_Equal(FAccountID AccountIDA, FAccountID AccountIDB) { return AccountIDA == AccountIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool AccountID_NotEqual(FAccountID AccountIDA, FAccountID AccountIDB) { return AccountIDA != AccountIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool ScreenshotHandle_Equal(FScreenshotHandle ScreenshotHandleA, FScreenshotHandle ScreenshotHandleB) { return ScreenshotHandleA == ScreenshotHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool ScreenshotHandle_NotEqual(FScreenshotHandle ScreenshotHandleA, FScreenshotHandle ScreenshotHandleB) { return ScreenshotHandleA != ScreenshotHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamFriendsGroupID_Equal(FSteamFriendsGroupID SteamFriendsGroupIDA, FSteamFriendsGroupID SteamFriendsGroupIDB) { return SteamFriendsGroupIDA == SteamFriendsGroupIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamFriendsGroupID_NotEqual(FSteamFriendsGroupID SteamFriendsGroupIDA, FSteamFriendsGroupID SteamFriendsGroupIDB) { return SteamFriendsGroupIDA != SteamFriendsGroupIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamInventoryResult_Equal(FSteamInventoryResult SteamInventoryResultA, FSteamInventoryResult SteamInventoryResultB) { return SteamInventoryResultA == SteamInventoryResultB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamInventoryResult_NotEqual(FSteamInventoryResult SteamInventoryResultA, FSteamInventoryResult SteamInventoryResultB) { return SteamInventoryResultA != SteamInventoryResultB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamItemDef_Equal(FSteamItemDef SteamItemDefA, FSteamItemDef SteamItemDefB) { return SteamItemDefA == SteamItemDefB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamItemDef_NotEqual(FSteamItemDef SteamItemDefA, FSteamItemDef SteamItemDefB) { return SteamItemDefA != SteamItemDefB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HServerQuery_Equal(FHServerQuery HServerQueryA, FHServerQuery HServerQueryB) { return HServerQueryA == HServerQueryB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HServerQuery_NotEqual(FHServerQuery HServerQueryA, FHServerQuery HServerQueryB) { return HServerQueryA != HServerQueryB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool InputActionSetHandle_Equal(FInputActionSetHandle InputActionSetHandleA, FInputActionSetHandle InputActionSetHandleB) { return InputActionSetHandleA == InputActionSetHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool InputActionSetHandle_NotEqual(FInputActionSetHandle InputActionSetHandleA, FInputActionSetHandle InputActionSetHandleB) { return InputActionSetHandleA != InputActionSetHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool InputAnalogActionHandle_Equal(FInputAnalogActionHandle InputAnalogActionHandleA, FInputAnalogActionHandle InputAnalogActionHandleB) { return InputAnalogActionHandleA == InputAnalogActionHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool InputAnalogActionHandle_NotEqual(FInputAnalogActionHandle InputAnalogActionHandleA, FInputAnalogActionHandle InputAnalogActionHandleB) { return InputAnalogActionHandleA != InputAnalogActionHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool InputDigitalActionHandle_Equal(FInputDigitalActionHandle InputDigitalActionHandleA, FInputDigitalActionHandle InputDigitalActionHandleB) { return InputDigitalActionHandleA == InputDigitalActionHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool InputDigitalActionHandle_NotEqual(FInputDigitalActionHandle InputDigitalActionHandleA, FInputDigitalActionHandle InputDigitalActionHandleB) { return InputDigitalActionHandleA != InputDigitalActionHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool InputHandle_Equal(FInputHandle InputHandleA, FInputHandle InputHandleB) { return InputHandleA == InputHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool InputHandle_NotEqual(FInputHandle InputHandleA, FInputHandle InputHandleB) { return InputHandleA != InputHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool PartyBeaconID_Equal(FPartyBeaconID PartyBeaconIDA, FPartyBeaconID PartyBeaconIDB) { return PartyBeaconIDA == PartyBeaconIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool PartyBeaconID_NotEqual(FPartyBeaconID PartyBeaconIDA, FPartyBeaconID PartyBeaconIDB) { return PartyBeaconIDA != PartyBeaconIDB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool UGCHandle_Equal(FUGCHandle UGCHandleA, FUGCHandle UGCHandleB) { return UGCHandleA == UGCHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool UGCHandle_NotEqual(FUGCHandle UGCHandleA, FUGCHandle UGCHandleB) { return UGCHandleA != UGCHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool PublishedFileId_Equal(FPublishedFileId PublishedFileIdA, FPublishedFileId PublishedFileIdB) { return PublishedFileIdA == PublishedFileIdB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool PublishedFileId_NotEqual(FPublishedFileId PublishedFileIdA, FPublishedFileId PublishedFileIdB) { return PublishedFileIdA != PublishedFileIdB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool PublishedFileUpdateHandle_Equal(FPublishedFileUpdateHandle PublishedFileUpdateHandleA, FPublishedFileUpdateHandle PublishedFileUpdateHandleB) { return PublishedFileUpdateHandleA == PublishedFileUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool PublishedFileUpdateHandle_NotEqual(FPublishedFileUpdateHandle PublishedFileUpdateHandleA, FPublishedFileUpdateHandle PublishedFileUpdateHandleB) { return PublishedFileUpdateHandleA != PublishedFileUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool UGCFileWriteStreamHandle_Equal(FUGCFileWriteStreamHandle UGCFileWriteStreamHandleA, FUGCFileWriteStreamHandle UGCFileWriteStreamHandleB) { return UGCFileWriteStreamHandleA == UGCFileWriteStreamHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool UGCFileWriteStreamHandle_NotEqual(FUGCFileWriteStreamHandle UGCFileWriteStreamHandleA, FUGCFileWriteStreamHandle UGCFileWriteStreamHandleB) { return UGCFileWriteStreamHandleA != UGCFileWriteStreamHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamLeaderboardEntries_Equal(FSteamLeaderboardEntries SteamLeaderboardEntriesA, FSteamLeaderboardEntries SteamLeaderboardEntriesB) { return SteamLeaderboardEntriesA == SteamLeaderboardEntriesB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamLeaderboardEntries_NotEqual(FSteamLeaderboardEntries SteamLeaderboardEntriesA, FSteamLeaderboardEntries SteamLeaderboardEntriesB) { return SteamLeaderboardEntriesA != SteamLeaderboardEntriesB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamLeaderboard_Equal(FSteamLeaderboard SteamLeaderboardA, FSteamLeaderboard SteamLeaderboardB) { return SteamLeaderboardA == SteamLeaderboardB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamLeaderboard_NotEqual(FSteamLeaderboard SteamLeaderboardA, FSteamLeaderboard SteamLeaderboardB) { return SteamLeaderboardA != SteamLeaderboardB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool UGCQueryHandle_Equal(FUGCQueryHandle UGCQueryHandleA, FUGCQueryHandle UGCQueryHandleB) { return UGCQueryHandleA == UGCQueryHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool UGCQueryHandle_NotEqual(FUGCQueryHandle UGCQueryHandleA, FUGCQueryHandle UGCQueryHandleB) { return UGCQueryHandleA != UGCQueryHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool UGCUpdateHandle_Equal(FUGCUpdateHandle UGCUpdateHandleA, FUGCUpdateHandle UGCUpdateHandleB) { return UGCUpdateHandleA == UGCUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool UGCUpdateHandle_NotEqual(FUGCUpdateHandle UGCUpdateHandleA, FUGCUpdateHandle UGCUpdateHandleB) { return UGCUpdateHandleA != UGCUpdateHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HTTPCookieContainerHandle_Equal(FHTTPCookieContainerHandle HTTPCookieContainerHandleA, FHTTPCookieContainerHandle HTTPCookieContainerHandleB) { return HTTPCookieContainerHandleA == HTTPCookieContainerHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HTTPCookieContainerHandle_NotEqual(FHTTPCookieContainerHandle HTTPCookieContainerHandleA, FHTTPCookieContainerHandle HTTPCookieContainerHandleB) { return HTTPCookieContainerHandleA != HTTPCookieContainerHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HTTPRequestHandle_Equal(FHTTPRequestHandle HTTPRequestHandleA, FHTTPRequestHandle HTTPRequestHandleB) { return HTTPRequestHandleA == HTTPRequestHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HTTPRequestHandle_NotEqual(FHTTPRequestHandle HTTPRequestHandleA, FHTTPRequestHandle HTTPRequestHandleB) { return HTTPRequestHandleA != HTTPRequestHandleB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool HHTMLBrowser_Equal(FHHTMLBrowser HHTMLBrowserA, FHHTMLBrowser HHTMLBrowserB) { return HHTMLBrowserA == HHTMLBrowserB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool HHTMLBrowser_NotEqual(FHHTMLBrowser HHTMLBrowserA, FHHTMLBrowser HHTMLBrowserB) { return HHTMLBrowserA != HHTMLBrowserB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamInputAnalogActionData_Equal(FSteamInputAnalogActionData SteamInputAnalogActionDataA, FSteamInputAnalogActionData SteamInputAnalogActionDataB) { return SteamInputAnalogActionDataA == SteamInputAnalogActionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamInputAnalogActionData_NotEqual(FSteamInputAnalogActionData SteamInputAnalogActionDataA, FSteamInputAnalogActionData SteamInputAnalogActionDataB) { return SteamInputAnalogActionDataA != SteamInputAnalogActionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamInputDigitalActionData_Equal(FSteamInputDigitalActionData SteamInputDigitalActionDataA, FSteamInputDigitalActionData SteamInputDigitalActionDataB) { return SteamInputDigitalActionDataA == SteamInputDigitalActionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamInputDigitalActionData_NotEqual(FSteamInputDigitalActionData SteamInputDigitalActionDataA, FSteamInputDigitalActionData SteamInputDigitalActionDataB) { return SteamInputDigitalActionDataA != SteamInputDigitalActionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamInputMotionData_Equal(FSteamInputMotionData SteamInputMotionDataA, FSteamInputMotionData SteamInputMotionDataB) { return SteamInputMotionDataA == SteamInputMotionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamInputMotionData_NotEqual(FSteamInputMotionData SteamInputMotionDataA, FSteamInputMotionData SteamInputMotionDataB) { return SteamInputMotionDataA != SteamInputMotionDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamItemDetails_Equal(FSteamItemDetails SteamItemDetailsA, FSteamItemDetails SteamItemDetailsB) { return SteamItemDetailsA == SteamItemDetailsB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamItemDetails_NotEqual(FSteamItemDetails SteamItemDetailsA, FSteamItemDetails SteamItemDetailsB) { return SteamItemDetailsA != SteamItemDetailsB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamPartyBeaconLocation_Equal(FSteamPartyBeaconLocation SteamPartyBeaconLocationA, FSteamPartyBeaconLocation SteamPartyBeaconLocationB) { return SteamPartyBeaconLocationA == SteamPartyBeaconLocationB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamPartyBeaconLocation_NotEqual(FSteamPartyBeaconLocation SteamPartyBeaconLocationA, FSteamPartyBeaconLocation SteamPartyBeaconLocationB) { return SteamPartyBeaconLocationA != SteamPartyBeaconLocationB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamLeaderboardEntry_Equal(FSteamLeaderboardEntry SteamLeaderboardEntryA, FSteamLeaderboardEntry SteamLeaderboardEntryB) { return SteamLeaderboardEntryA == SteamLeaderboardEntryB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamLeaderboardEntry_NotEqual(FSteamLeaderboardEntry SteamLeaderboardEntryA, FSteamLeaderboardEntry SteamLeaderboardEntryB) { return SteamLeaderboardEntryA != SteamLeaderboardEntryB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamUGCDetails_Equal(FSteamUGCDetails SteamUGCDetailsA, FSteamUGCDetails SteamUGCDetailsB) { return SteamUGCDetailsA == SteamUGCDetailsB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamUGCDetails_NotEqual(FSteamUGCDetails SteamUGCDetailsA, FSteamUGCDetails SteamUGCDetailsB) { return SteamUGCDetailsA != SteamUGCDetailsB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "equal == identical is", CompactNodeTitle = "=="))
	static bool SteamItemPriceData_Equal(FSteamItemPriceData SteamItemPriceDataA, FSteamItemPriceData SteamItemPriceDataB) { return SteamItemPriceDataA == SteamItemPriceDataB; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (Keywords = "notequal != notidentical not", CompactNodeTitle = "!="))
	static bool SteamItemPriceData_NotEqual(FSteamItemPriceData SteamItemPriceDataA, FSteamItemPriceData SteamItemPriceDataB) { return SteamItemPriceDataA != SteamItemPriceDataB; }

	/** Make and Breaks */

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeMakeFunc))
	static FSteamID MakeSteamID(const FString& SteamID);

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeBreakFunc))
	static FString BreakSteamID(FSteamID SteamID) { return LexToString(SteamID.Value); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeMakeFunc))
	static FAppID MakeAppID(int32 AppID) { return {(uint32)AppID}; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeBreakFunc))
	static int32 BreakAppID(FAppID AppID) { return (int32)AppID; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeMakeFunc))
	static FAccountID MakeAccountID(int32 AccountID) { return {(uint32)AccountID}; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeBreakFunc))
	static int32 BreakAccountID(FAccountID AccountID) { return (int32)AccountID; }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamBPUtils", meta = (NativeMakeFunc))
	static FPublishedFileId MakePublishedFileId(const FString& FileID);

protected:
private:
};

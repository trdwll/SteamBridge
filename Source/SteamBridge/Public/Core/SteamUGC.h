// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

#include "SteamUGC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddAppDependencyResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddUGCDependencyResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, FPublishedFileId, ChildPublishedFileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreateItemResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, bool, bUserNeedsToAcceptWorkshopLegalAgreement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDownloadItemResultDelegate, int32, AppID, FPublishedFileId, PublishedFileID, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnGetAppDependenciesResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, const TArray<int32>&, AppID, int32, NumAppDependencies, int32, TotalNumAppDependencies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeleteItemResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnGetUserItemVoteResultDelegate, FPublishedFileId, PublishedFileID, ESteamResult, Result, bool, bVotedUp, bool, bVotedDown, bool, bVoteSkipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemInstalledDelegate, int32, AppID, FPublishedFileId, PublishedFileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoveAppDependencyResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoveUGCDependencyResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID, FPublishedFileId, ChildPublishedFileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetUserItemVoteResultDelegate, FPublishedFileId, PublishedFileID, ESteamResult, Result, bool, bVoteUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartPlaytimeTrackingResultDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnSteamUGCQueryCompletedDelegate, FUGCQueryHandle, Handle, ESteamResult, Result, int32, NumResultsReturned, int32, TotalMatchingResults, bool, bCachedData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopPlaytimeTrackingResultDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubmitItemUpdateResultDelegate, ESteamResult, Result, bool, bUserNeedsToAcceptWorkshopLegalAgreement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUserFavoriteItemsListChangedDelegate, FPublishedFileId, PublishedFileID, ESteamResult, Result, bool, bWasAddRequest);

/**
 * Functions to create, consume, and interact with the Steam Workshop.
 * https://partner.steamgames.com/doc/api/ISteamUGC
 */
UCLASS()
class STEAMBRIDGE_API USteamUGC final : public UObject
{
	GENERATED_BODY()

public:
	USteamUGC();
	~USteamUGC();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam UGC", CompactNodeTitle = "SteamUGC"))
	static USteamUGC* GetSteamUGC() { return USteamUGC::StaticClass()->GetDefaultObject<USteamUGC>(); }

	/**
	 * Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies. This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
	 *
	 * @param FPublishedFileId PublishedFileID - The item.
	 * @param int32 AppID - The required app/dlc.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a AddAppDependencyResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall AddAppDependency(FPublishedFileId PublishedFileID, int32 AppID) const { return SteamUGC()->AddAppDependency(PublishedFileID, AppID); }

	/**
	 * Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection. Otherwise,
	 * the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
	 *
	 * @param FPublishedFileId ParentPublishedFileID - The workshop item to add a dependency to.
	 * @param FPublishedFileId ChildPublishedFileID - The dependency to add to the parent.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a AddUGCDependencyResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall AddDependency(FPublishedFileId ParentPublishedFileID, FPublishedFileId ChildPublishedFileID) const { return SteamUGC()->AddDependency(ParentPublishedFileID, ChildPublishedFileID); }

	/**
	 * Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & TagName - The tag that must NOT be attached to the UGC to receive it.
	 * @return bool - true upon success. false if the UGC query handle is invalid, if the UGC query handle is from CreateQueryUGCDetailsRequest, or pTagName was NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddExcludedTag(FUGCQueryHandle handle, const FString& TagName) const { return SteamUGC()->AddExcludedTag(handle, TCHAR_TO_UTF8(*TagName)); }

	/**
	 * Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
	 * Key names are restricted to alpha-numeric characters and the '_' character.
	 * Both keys and values cannot exceed 255 characters in length.
	 * Key-value tags are searchable by exact match only.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & Key - The key to set on the item.
	 * @param const FString & Value - A value to map to the key.
	 * @return bool - true upon success. false if the UGC update handle is invalid, if pchKey or pchValue invalid because they are NULL or have exceeded the maximum length, or if you are trying to add more than 100 key-value tags in a single update.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddItemKeyValueTag(FUGCUpdateHandle handle, const FString& Key, const FString& Value) const { return SteamUGC()->AddItemKeyValueTag(handle, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Adds an additional preview file for the item.
	 * Then the format of the image should be one that both the web and the application (if necessary) can render, and must be under 1MB. Suggested formats include JPG, PNG and GIF.
	 * NOTE: Using k_EItemPreviewType_YouTubeVideo or k_EItemPreviewType_Sketchfab are not currently supported with this API. For YouTube videos you should use AddItemPreviewVideo.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & PreviewFile - Absolute path to the local image.
	 * @param ESteamItemPreviewType type - The type of this preview.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddItemPreviewFile(FUGCUpdateHandle handle, const FString& PreviewFile, ESteamItemPreviewType type) const { return SteamUGC()->AddItemPreviewFile(handle, TCHAR_TO_UTF8(*PreviewFile), (EItemPreviewType)type); }

	/**
	 * Adds an additional video preview from YouTube for the item.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & VideoID - The YouTube video ID to add. (e.g. "jHgZh4GV9G0")
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddItemPreviewVideo(FUGCUpdateHandle handle, const FString& VideoID) const { return SteamUGC()->AddItemPreviewVideo(handle, TCHAR_TO_UTF8(*VideoID)); }

	/**
	 * Adds a workshop item to the users favorites list.
	 *
	 * @param int32 AppId - The app ID that this item belongs to.
	 * @param FPublishedFileId PublishedFileID - The workshop item to add to the users favorites list.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a UserFavoriteItemsListChanged_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall AddItemToFavorites(int32 AppId, FPublishedFileId PublishedFileID) const { return SteamUGC()->AddItemToFavorites(AppId, PublishedFileID); }

	/**
	 * Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & Key - The key-value key that must be attached to the UGC to receive it.
	 * @param const FString & Value - The key-value value associated with pKey that must be attached to the UGC to receive it.
	 * @return bool - true upon success. false if the UGC query handle is invalid or if pKey or pValue are NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddRequiredKeyValueTag(FUGCQueryHandle handle, const FString& Key, const FString& Value) const { return SteamUGC()->AddRequiredKeyValueTag(handle, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & TagName - The tag that must be attached to the UGC to receive it.
	 * @return bool - true upon success. false if the UGC query handle is invalid, if the UGC query handle is from CreateQueryUGCDetailsRequest, or pTagName was NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddRequiredTag(FUGCQueryHandle handle, const FString& TagName) const { return SteamUGC()->AddRequiredTag(handle, TCHAR_TO_UTF8(*TagName)); }

	/**
	 * Adds the requirement that the returned items from the pending UGC Query have at least one of the tags in the given set (logical "or"). For each tag group that is added, at least one tag from each group is required to be on the matching items.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const TArray<FString> & Tags - A set of tags where at least one of the tags must attached to the UGC.
	 * @return bool - true upon success. false if the UGC query handle is invalid, if the UGC query handle is from CreateQueryUGCDetailsRequest, or pTagName was NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool AddRequiredTagGroup(FUGCQueryHandle handle, const TArray<FString>& Tags) const;

	/**
	 * Lets game servers set a specific workshop folder before issuing any UGC commands.
	 * This is helpful if you want to support multiple game servers running out of the same install folder.
	 *
	 * @param int32 WorkshopDepotID - The depot ID of the game server.
	 * @param const FString & FolderName - The absolute path to store the workshop content.
	 * @return bool - true upon success; otherwise, false if the calling user is not a game server or if the workshop is currently updating it's content.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool BInitWorkshopForGameServer(int32 WorkshopDepotID, const FString& FolderName) const { return SteamUGC()->BInitWorkshopForGameServer(WorkshopDepotID, TCHAR_TO_UTF8(*FolderName)); }

	/**
	 * Creates a new workshop item with no content attached yet.
	 *
	 * @param int32 ConsumerAppId - The App ID that will be using this item.
	 * @param ESteamWorkshopFileType FileType - The type of UGC to create.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a CreateItemResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall CreateItem(int32 ConsumerAppId, ESteamWorkshopFileType FileType) const { return SteamUGC()->CreateItem(ConsumerAppId, (EWorkshopFileType)FileType); }

	/**
	 * Query for all matching UGC. You can use this to list all of the available UGC for your app.
	 * This will return up to 50 results as declared by kNumUGCResultsPerPage. You can make subsequent calls to this function, increasing the unPage each time to get the next set of results.
	 * NOTE: Either nConsumerAppID or nCreatorAppID must have a valid AppID!
	 * NOTE: You must release the handle returned by this function by calling ReleaseQueryUGCRequest when you are done with it!
	 * To query for the UGC associated with a single user you can use CreateQueryUserUGCRequest instead.
	 *
	 * @param ESteamUGCQuery QueryType - Used to specify the sorting and filtering for this call.
	 * @param EUGCMatchingUGCType MatchingUGCTypeFileType - Used to specify the type of UGC queried for.
	 * @param int32 CreatorAppID - 	This should contain the App ID of the app where the item was created. This may be different than nConsumerAppID if your item creation tool is a seperate App ID.
	 * @param int32 ConsumerAppID - This should contain the App ID for the current game or application. Do not pass the App ID of the workshop item creation tool if that is a separate App ID!
	 * @param int32 Page - The page number of the results to receive. This should start at 1 on the first call.
	 * @return FUGCQueryHandle - Returns a new UGCQueryHandle_t upon success, and k_UGCQueryHandleInvalid in the following situations:
	 * Either nCreatorAppID or nConsumerAppID is not set to the currently running app.
	 * unPage was less than 1.
	 * An internal error occurred.
	 * This handle can be used to further customize the query before sending it out with SendQueryUGCRequest.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FUGCQueryHandle CreateQueryAllUGCRequest(ESteamUGCQuery QueryType, ESteamUGCMatchingUGCType MatchingUGCTypeFileType, int32 CreatorAppID, int32 ConsumerAppID, int32 Page) const;

	/**
	 * Query for the details of specific workshop items.
	 * This will return up to 50 results as declared by kNumUGCResultsPerPage.
	 * NOTE: Either nConsumerAppID or nCreatorAppID must have a valid AppID!
	 * NOTE: You must release the handle returned by this function by calling ReleaseQueryUGCRequest when you are done with it!
	 * To query all the UGC for your app you can use CreateQueryAllUGCRequest instead.
	 *
	 * @param TArray<FPublishedFileId> & PublishedFileIDs - The list of workshop items to get the details for.
	 * @param int32 NumPublishedFileIDs - The number of items in pvecPublishedFileID.
	 * @return FUGCQueryHandle - Returns a new UGCQueryHandle_t upon success, and k_UGCQueryHandleInvalid in the following situations:
	 * unNumPublishedFileIDs is less than 1.
	 * An internal error occurred.
	 * This handle can be used to further customize the query before sending it out with SendQueryUGCRequest.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FUGCQueryHandle CreateQueryUGCDetailsRequest(TArray<FPublishedFileId> PublishedFileIDs, int32 NumPublishedFileIDs) const { return SteamUGC()->CreateQueryUGCDetailsRequest((PublishedFileId_t*)PublishedFileIDs.GetData(), NumPublishedFileIDs); }

	/**
	 * Query UGC associated with a user. You can use this to list the UGC the user is subscribed to amongst other things.
	 * This will return up to 50 results as declared by kNumUGCResultsPerPage. You can make subsequent calls to this function, increasing the unPage each time to get the next set of results.
	 * NOTE: Either nConsumerAppID or nCreatorAppID must have a valid AppID!
	 * NOTE: You must release the handle returned by this function by calling ReleaseQueryUGCRequest when you are done with it!
	 * To query all the UGC for your app you can use CreateQueryAllUGCRequest instead.
	 *
	 * @param FAccountID AccountID - The Account ID to query the UGC for. You can use CSteamID.GetAccountID to get the Account ID from a Steam ID.
	 * @param ESteamUserUGCList ListType - Used to specify the type of list to get. If the currently logged in user is different than the user specified in unAccountID, then some options are not be allowed.
	 *  (k_EUserUGCList_VotedOn, k_EUserUGCList_VotedUp, k_EUserUGCList_VotedDown, k_EUserUGCList_WillVoteLater, k_EUserUGCList_Subscribed)
	 * @param ESteamUGCMatchingUGCType MatchingUGCType - Used to specify the type of UGC queried for.
	 * @param ESteamUserUGCListSortOrder SortOrder - Used to specify the order that the list will be sorted in.
	 * @param int32 CreatorAppID - This should contain the App ID of the app where the item was created. This may be different than nConsumerAppID if your item creation tool is a seperate App ID.
	 * @param int32 ConsumerAppID - This should contain the App ID for the current game or application. Do not pass the App ID of the workshop item creation tool if that is a separate App ID!
	 * @param int32 Page - The page number of the results to receive. This should start at 1 on the first call.
	 * @return FUGCQueryHandle - Returns a new UGCQueryHandle_t upon success, and k_UGCQueryHandleInvalid in the following situations:
	 * Either nCreatorAppID or nConsumerAppID is not set to the currently running app.
	 * unPage was less than 1.
	 * The given eListType is not supported for users other than the one requesting the details.
	 * An internal error occurred.
	 * This handle can be used to further customize the query before sending it out with SendQueryUGCRequest.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FUGCQueryHandle CreateQueryUserUGCRequest(FAccountID AccountID, ESteamUserUGCList ListType, ESteamUGCMatchingUGCType MatchingUGCType, ESteamUserUGCListSortOrder SortOrder, int32 CreatorAppID, int32 ConsumerAppID, int32 Page) const;

	/**
	 * Deletes the item without prompting the user.
	 *
	 * @param FPublishedFileId PublishedFileID - The item to delete.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a DeleteItemResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall DeleteItem(FPublishedFileId PublishedFileID) const { return SteamUGC()->DeleteItem(PublishedFileID); }

	/**
	 * Download or update a workshop item.
	 * If the return value is true then register and wait for the Callback DownloadItemResult_t before calling GetItemInstallInfo or accessing the workshop item on disk.
	 * If the user is not subscribed to the item (e.g. a Game Server using anonymous login), the workshop item will be downloaded and cached temporarily.
	 * If the workshop item has an item state of k_EItemStateNeedsUpdate, then this function can be called to initiate the update. Do not access the workshop item on disk until the Callback DownloadItemResult_t is called.
	 * The DownloadItemResult_t callback contains the app ID associated with the workshop item. It should be compared against the running app ID as the handler will be called for all item downloads regardless of the running application.
	 * Triggers a DownloadItemResult_t callback.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to download.
	 * @param bool bHighPriority - Start the download in high priority mode, pausing any existing in-progress Steam downloads and immediately begin downloading this workshop item.
	 * @return bool - true if the download was successfully started; otherwise, false if nPublishedFileID is invalid or the user is not logged on.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool DownloadItem(FPublishedFileId PublishedFileID, bool bHighPriority) const { return SteamUGC()->DownloadItem(PublishedFileID, bHighPriority); }

	/**
	 * Get the app dependencies associated with the given PublishedFileId_t. These are "soft" dependencies that are shown on the web. It is up to the application to determine whether an item can be used or not.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to get app dependencies for.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a GetAppDependenciesResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall GetAppDependencies(FPublishedFileId PublishedFileID) const { return SteamUGC()->GetAppDependencies(PublishedFileID); }

	/**
	 * Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to get the download info for.
	 * @param int64 & BytesDownloaded - Returns the current bytes downloaded.
	 * @param int64 & BytesTotal - Returns the total bytes. This is only valid after the download has started.
	 * @return bool - true if the download information was available; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetItemDownloadInfo(FPublishedFileId PublishedFileID, int64& BytesDownloaded, int64& BytesTotal) const { return SteamUGC()->GetItemDownloadInfo(PublishedFileID, (uint64*)&BytesDownloaded, (uint64*)&BytesTotal); }

	/**
	 * Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
	 * Calling this sets the "used" flag on the workshop item for the current player and adds it to their k_EUserUGCList_UsedOrPlayed list.
	 * If k_EItemStateLegacyItem is set then pchFolder contains the path to the legacy file itself, not a folder.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to get the install info for.
	 * @param int64 & SizeOnDisk - Returns the size of the workshop item in bytes.
	 * @param FString & FolderName - Returns the absolute path to the folder containing the content by copying it.
	 * @param int32 FolderSize - The size of pchFolder in bytes.
	 * @param FDateTime & TimeStamp - Returns the time when the workshop item was last updated.
	 * @return bool - true if the workshop item is already installed. false in the following cases:
	 * cchFolderSize is 0.
	 * The workshop item has no content.
	 * The workshop item is not installed.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetItemInstallInfo(FPublishedFileId PublishedFileID, int64& SizeOnDisk, FString& FolderName, int32 FolderSize, FDateTime& TimeStamp) const;

	/**
	 * Gets the current state of a workshop item on this client.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to get the state for.
	 * @return int32 - Returns the item state. Should be used with the EItemState flags to determine the state of the workshop item.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	int32 GetItemState(FPublishedFileId PublishedFileID) const { return SteamUGC()->GetItemState(PublishedFileID); }

	/**
	 * Gets the progress of an item update.
	 *
	 * @param FUGCUpdateHandle handle - The update handle to get the progress for.
	 * @param int64 & BytesProcessed - Returns the current number of bytes uploaded.
	 * @param int64 & BytesTotal - Returns the total number of bytes that will be uploaded.
	 * @return ESteamItemUpdateStatus - The current status.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	ESteamItemUpdateStatus GetItemUpdateProgress(FUGCUpdateHandle handle, int64& BytesProcessed, int64& BytesTotal) const { return (ESteamItemUpdateStatus)SteamUGC()->GetItemUpdateProgress(handle, (uint64*)BytesProcessed, (uint64*)BytesTotal); }

	/**
	 * Gets the total number of items the current user is subscribed to for the game or application.
	 *
	 * @return int32 - Returns 0 if called from a game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	int32 GetNumSubscribedItems() const { return SteamUGC()->GetNumSubscribedItems(); }

	/**
	 * Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * Before calling this you should call GetQueryUGCNumAdditionalPreviews to get number of additional previews.
	 *
	 * @param FUGCQueryHandle handle - 	The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param int32 previewIndex - The index of the additional preview to get the details of.
	 * @param FString & URLOrVideoID - Returns a URL or Video ID by copying it into this string.
	 * @param int32 URLSize - The size of pchURLOrVideoID in bytes.
	 * @param FString & OriginalFileName - Returns the original file name. May be set to NULL to not receive this.
	 * @param int32 OriginalFileNameSize - The size of pchOriginalFileName in bytes.
	 * @param ESteamItemPreviewType & PreviewType - The type of preview that was returned.
	 * @return bool - true upon success, indicates that pchURLOrVideoID and pPreviewType have been filled out. Otherwise, false if the UGC query handle is invalid, the index is out of bounds, or previewIndex is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCAdditionalPreview(FUGCQueryHandle handle, int32 index, int32 previewIndex, FString& URLOrVideoID, int32 URLSize, FString& OriginalFileName, int32 OriginalFileNameSize, ESteamItemPreviewType& PreviewType) const;

	/**
	 * Retrieve the ids of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see AddDependency).
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * You should create pvecPublishedFileID with m_unNumChildren provided in SteamUGCDetails_t after getting the UGC details with GetQueryUGCResult.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param TArray<FPublishedFileId> & PublishedFileIDs - Returns the UGC children by setting this array.
	 * @param int32 MaxEntries - The length of pvecPublishedFileID.
	 * @return bool - true upon success, indicates that pvecPublishedFileID has been filled out. Otherwise, false if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCChildren(FUGCQueryHandle handle, int32 index, TArray<FPublishedFileId>& PublishedFileIDs, int32 MaxEntries) const;

	// #TODO: GetQueryUGCNumTags (available in sdk 1.51)
	// #TODO: GetQueryUGCTag (available in sdk 1.51)
	// #TODO: GetQueryUGCTagDisplayName (available in sdk 1.51)

	/**
	 * Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * Before calling this you should call GetQueryUGCNumKeyValueTags to get number of tags.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param int32 keyValueTagIndex - The index of the tag to get the details of.
	 * @param FString & Key - 	Returns the key by copying it into this string.
	 * @param int32 KeySize - The size of pchKey in bytes.
	 * @param FString & Value - Returns the value by copying it into this string.
	 * @param int32 ValueSize - The size of pchValue in bytes.
	 * @return bool - true upon success, indicates that pchKey and pchValue have been filled out. Otherwise, false if the UGC query handle is invalid, the index is out of bounds, or keyValueTagIndex is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCKeyValueTag(FUGCQueryHandle handle, int32 index, int32 keyValueTagIndex, FString& Key, int32 KeySize, FString& Value, int32 ValueSize) const;

	/**
	 * Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param FString & Metadata - Returns the url by copying it into this string.
	 * @param int32 Metadatasize - The size of pchMetadata in bytes.
	 * @return bool - true upon success, indicates that pchMetadata has been filled out. Otherwise, false if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCMetadata(FUGCQueryHandle handle, int32 index, FString& Metadata, int32 Metadatasize) const;

	/**
	 * Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * You can then call GetQueryUGCAdditionalPreview to get the details of each additional preview.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @return int32 - The number of additional previews associated with the specified workshop item. Returns 0 if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	int32 GetQueryUGCNumAdditionalPreviews(FUGCQueryHandle handle, int32 index) const { return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index); }

	/**
	 * Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * You can then call GetQueryUGCKeyValueTag to get the details of each tag.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @return int32 - The number of key-value tags associated with the specified workshop item. Returns 0 if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	int32 GetQueryUGCNumKeyValueTags(FUGCQueryHandle handle, int32 index) const { return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index); }

	/**
	 * Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 * You can use this URL to download and display the preview image instead of having to download it using the m_hPreviewFile in the ISteamUGC::SteamUGCDetails_t struct.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param FString & URL - Returns the url by copying it into this string.
	 * @param int32 URLSize - The size of pchURL in bytes.
	 * @return bool - true upon success, indicates that pchURL has been filled out. Otherwise, false if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCPreviewURL(FUGCQueryHandle handle, int32 index, FString& URL, int32 URLSize) const;

	/**
	 * Retrieve the details of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param FSteamUGCDetails & Details - Returns the the UGC details.
	 * @return bool - true upon success, indicates that pDetails has been filled out. Otherwise, false if the UGC query handle is invalid or the index is out of bounds.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCResult(FUGCQueryHandle handle, int32 index, FSteamUGCDetails& Details) const;

	/**
	 * Retrieve various statistics of an individual workshop item after receiving a querying UGC call result.
	 * You should call this in a loop to get the details of all the workshop items returned.
	 * NOTE: This must only be called with the handle obtained from a successful SteamUGCQueryCompleted_t call result.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to get the results from.
	 * @param int32 index - The index of the item to get the details of.
	 * @param ESteamItemStatistic StatType - The statistic to retrieve.
	 * @param int64 & StatValue - Returns the value associated with the specified statistic.
	 * @return bool - true upon success, indicates that pStatValue has been filled out. Otherwise, false if the UGC query handle is invalid, the index is out of bounds, or eStatType was invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetQueryUGCStatistic(FUGCQueryHandle handle, int32 index, ESteamItemStatistic StatType, int64& StatValue) const { return SteamUGC()->GetQueryUGCStatistic(handle, index, (EItemStatistic)StatType, (uint64*)&StatValue); }

	/**
	 * Gets a list of all of the items the current user is subscribed to for the current game.
	 * You create an array with the size provided by GetNumSubscribedItems before calling this.
	 *
	 * @param TArray<FPublishedFileId> & PublishedFileIDs - The array where the item ids will be copied into.
	 * @param int32 MaxEntries - The maximum number of items to return. This should typically be the same as GetNumSubscribedItems and the same size as pvecPublishedFileID.
	 * @return int32 - The number of subscribed workshop items that were populated into pvecPublishedFileID. Returns 0 if called from a game server.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	int32 GetSubscribedItems(TArray<FPublishedFileId>& PublishedFileIDs, int32 MaxEntries) const;

	/**
	 * Gets the users vote status on a workshop item.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item ID to get the users vote.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a GetUserItemVoteResult_t call result.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	FSteamAPICall GetUserItemVote(FPublishedFileId PublishedFileID) const { return SteamUGC()->GetUserItemVote(PublishedFileID); }

	/**
	 * Releases a UGC query handle when you are done with it to free up memory.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to release.
	 * @return bool - Always returns true.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool ReleaseQueryUGCRequest(FUGCQueryHandle handle) const { return SteamUGC()->ReleaseQueryUGCRequest(handle); }

	/**
	 * Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
	 *
	 * @param FPublishedFileId PublishedFileID - The item.
	 * @param int32 AppID - The app/dlc.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a RemoveAppDependencyResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall RemoveAppDependency(FPublishedFileId PublishedFileID, int32 AppID) const { return SteamUGC()->RemoveAppDependency(PublishedFileID, AppID); }

	/**
	 * Removes a workshop item as a dependency from the specified item.
	 *
	 * @param FPublishedFileId ParentPublishedFileID - The workshop item to remove a dependency from.
	 * @param FPublishedFileId ChildPublishedFileID - The dependency to remove from the parent.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a RemoveUGCDependencyResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall RemoveDependency(FPublishedFileId ParentPublishedFileID, FPublishedFileId ChildPublishedFileID) const { return SteamUGC()->RemoveDependency(ParentPublishedFileID, ChildPublishedFileID); }

	/**
	 * Removes a workshop item from the users favorites list.
	 *
	 * @param int32 AppId - The app ID that this item belongs to.
	 * @param FPublishedFileId PublishedFileID - The workshop item to remove from the users favorites list.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a UserFavoriteItemsListChanged_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall RemoveItemFromFavorites(int32 AppId, FPublishedFileId PublishedFileID) const { return SteamUGC()->RemoveItemFromFavorites(AppId, PublishedFileID); }

	/**
	 * Removes an existing key value tag from an item.
	 * You can only call this up to 100 times per item update. If you need remove more tags than that you'll need to make subsequent item updates.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & Key - The key to remove from the item.
	 * @return bool - true upon success. false if the UGC update handle is invalid or if you are trying to remove more than 100 key-value tags in a single update.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool RemoveItemKeyValueTags(FUGCUpdateHandle handle, const FString& Key) const { return SteamUGC()->RemoveItemKeyValueTags(handle, TCHAR_TO_UTF8(*Key)); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool RemoveItemPreview(FUGCUpdateHandle handle, int32 index) const { return SteamUGC()->RemoveItemPreview(handle, index); }

	/**
	 * Send a UGC query to Steam.
	 * This must be called with a handle obtained from CreateQueryUserUGCRequest, CreateQueryAllUGCRequest, or CreateQueryUGCDetailsRequest to actually send the request to Steam. Before calling this you should
	 * use one or more of the following APIs to customize your query:
	 * AddRequiredTag, AddExcludedTag, SetReturnOnlyIDs, SetReturnKeyValueTags, SetReturnLongDescription, SetReturnMetadata, SetReturnChildren, SetReturnAdditionalPreviews, SetReturnTotalOnly, SetLanguage,
	 * SetAllowCachedResponse, SetCloudFileNameFilter, SetMatchAnyTag, SetSearchText, SetRankedByTrendDays, AddRequiredKeyValueTag
	 *
	 * @param FUGCQueryHandle handle - The UGC query request handle to send.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a SteamUGCQueryCompleted_t call result. Returns k_uAPICallInvalid if the UGC query handle was invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall SendQueryUGCRequest(FUGCQueryHandle handle) const { return SteamUGC()->SendQueryUGCRequest(handle); }

	/**
	 * Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param int32 MaxAgeSeconds - The maximum amount of time that an item can be returned without a cache invalidation.
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetAllowCachedResponse(FUGCQueryHandle handle, int32 MaxAgeSeconds) const { return SteamUGC()->SetAllowCachedResponse(handle, MaxAgeSeconds); }

	/**
	 * Sets to only return items that have a specific filename on a pending UGC Query.
	 * NOTE: This can only be used with CreateQueryUserUGCRequest!
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & MatchCloudFileName - The filename to match.
	 * @return bool - true upon success. false if the UGC query handle is invalid, if the UGC query handle is not from CreateQueryUserUGCRequest or if pMatchCloudFileName is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetCloudFileNameFilter(FUGCQueryHandle handle, const FString& MatchCloudFileName) const { return SteamUGC()->SetCloudFileNameFilter(handle, TCHAR_TO_UTF8(*MatchCloudFileName)); }

	/**
	 * Sets the folder that will be stored as the content for an item.
	 * For efficient upload and download, files should not be merged or compressed into single files (e.g. zip files).
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & ContentFolder - The absolute path to a local folder containing the content for the item.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemContent(FUGCUpdateHandle handle, const FString& ContentFolder) const { return SteamUGC()->SetItemContent(handle, TCHAR_TO_UTF8(*ContentFolder)); }

	/**
	 * Sets a new description for an item.
	 * The description must be limited to the length defined by k_cchPublishedDocumentDescriptionMax.
	 * You can set what language this is for by using SetItemUpdateLanguage, if no language is set then "english" is assumed.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & Description - The new description of the item.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemDescription(FUGCUpdateHandle handle, const FString& Description) const { return SteamUGC()->SetItemDescription(handle, TCHAR_TO_UTF8(*Description)); }

	/**
	 * Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
	 * The metadata must be limited to the size defined by k_cchDeveloperMetadataMax.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & MetaData - The new metadata for this item.
	 * @return bool - true upon success. false if the UGC update handle is invalid, or if pchMetadata is longer than k_cchDeveloperMetadataMax.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemMetadata(FUGCUpdateHandle handle, const FString& MetaData) const { return SteamUGC()->SetItemMetadata(handle, TCHAR_TO_UTF8(*MetaData)); }

	/**
	 * Sets the primary preview image for the item.
	 * The format should be one that both the web and the application (if necessary) can render. Suggested formats include JPG, PNG and GIF.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & PreviewFile - The absolute path to a local preview image file for the item.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemPreview(FUGCUpdateHandle handle, const FString& PreviewFile) const { return SteamUGC()->SetItemPreview(handle, TCHAR_TO_UTF8(*PreviewFile)); }

	/**
	 * Sets arbitrary developer specified tags on an item.
	 * Each tag must be limited to 255 characters. Tag names can only include printable characters, excluding ','. For reference on what characters are allowed, refer to http://en.cppreference.com/w/c/string/byte/isprint
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle UpdateHandle - The workshop item update handle to customize.
	 * @param const TArray<FString> & Tags - The list of tags to set on this item.
	 * @return bool - true upon success. false if the UGC update handle is invalid, or if one of the tags is invalid either due to exceeding the maximum length or because it is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemTags(FUGCUpdateHandle UpdateHandle, const TArray<FString>& Tags) const;

	/**
	 * Sets a new title for an item.
	 * The title must be limited to the size defined by k_cchPublishedDocumentTitleMax.
	 * You can set what language this is for by using SetItemUpdateLanguage, if no language is set then "english" is assumed.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & Title - The new title of the item.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemTitle(FUGCUpdateHandle handle, const FString& Title) const { return SteamUGC()->SetItemTitle(handle, TCHAR_TO_UTF8(*Title)); }

	/**
	 * Sets the language of the title and description that will be set in this item update.
	 * This must be in the format of the API language code.
	 * If this is not set then "english" is assumed.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param const FString & Language - The language of the title and description that will be set in this update.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemUpdateLanguage(FUGCUpdateHandle handle, const FString& Language) const { return SteamUGC()->SetItemUpdateLanguage(handle, TCHAR_TO_UTF8(*Language)); }

	/**
	 * Sets the visibility of an item.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param ESteamRemoteStoragePublishedFileVisibility Visibility - The visibility to set.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetItemVisibility(FUGCUpdateHandle handle, ESteamRemoteStoragePublishedFileVisibility Visibility) const { return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)Visibility); }

	/**
	 * Sets the language to return the title and description in for the items on a pending UGC Query.
	 * This must be in the format of the API Language code.
	 * If this is not set then "english" is assumed.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & Language - The language to return.
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetLanguage(FUGCQueryHandle handle, const FString& Language) const { return SteamUGC()->SetLanguage(handle, TCHAR_TO_UTF8(*Language)); }

	/**
	 * Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.
	 * NOTE: This can only be used with CreateQueryAllUGCRequest!
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bMatchAnyTag - Should the item just need to have one required tag (true), or all of them? (false)
	 * @return bool - true upon success. false if the UGC query handle is invalid or if the UGC query handle is not from CreateQueryAllUGCRequest.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetMatchAnyTag(FUGCQueryHandle handle, bool bMatchAnyTag) const { return SteamUGC()->SetMatchAnyTag(handle, bMatchAnyTag); }

	/**
	 * Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.
	 * NOTE: This can only be used with CreateQueryAllUGCRequest!
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param int32 Days - Sets the number of days to rank items over. Valid values are 1-365 inclusive.
	 * @return bool - true upon success.
	 * false if the UGC query handle is invalid, if the UGC query handle is not from CreateQueryAllUGCRequest or if the EUGCQuery of the query is not one of:
	 * k_PublishedFileQueryType_RankedByTrend, k_PublishedFileQueryType_RankedByPlaytimeTrend, k_PublishedFileQueryType_RankedByAveragePlaytimeTrend, k_PublishedFileQueryType_RankedByVotesUp, or k_PublishedFileQueryType_RankedByPlaytimeSessionsTrend
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetRankedByTrendDays(FUGCQueryHandle handle, int32 Days) const { return SteamUGC()->SetRankedByTrendDays(handle, Days); }

	/**
	 * Sets whether to return any additional images/videos attached to the items on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnAdditionalPreviews - Return the additional previews for the items?
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnAdditionalPreviews(FUGCQueryHandle handle, bool bReturnAdditionalPreviews) const { return SteamUGC()->SetReturnAdditionalPreviews(handle, bReturnAdditionalPreviews); }

	/**
	 * Sets whether to return the IDs of the child items of the items on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnChildren - Return the IDs of children of the items?
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnChildren(FUGCQueryHandle handle, bool bReturnChildren) const { return SteamUGC()->SetReturnChildren(handle, bReturnChildren); }

	/**
	 * Sets whether to return any key-value tags for the items on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnKeyValueTags - Return any key-value tags for the items?
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnKeyValueTags(FUGCQueryHandle handle, bool bReturnKeyValueTags) const { return SteamUGC()->SetReturnKeyValueTags(handle, bReturnKeyValueTags); }

	/**
	 * Sets whether to return the full description for the items on a pending UGC Query.
	 * If you don't set this then you only receive the summary which is the description truncated at 255 bytes.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnLongDescription - Return the long description for the items?
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnLongDescription(FUGCQueryHandle handle, bool bReturnLongDescription) const { return SteamUGC()->SetReturnLongDescription(handle, bReturnLongDescription); }

	/**
	 * Sets whether to return the developer specified metadata for the items on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnMetadata - Return the metadata for the items?
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnMetadata(FUGCQueryHandle handle, bool bReturnMetadata) const { return SteamUGC()->SetReturnMetadata(handle, bReturnMetadata); }

	/**
	 * Sets whether to only return IDs instead of all the details on a pending UGC Query.
	 * This is useful for when you don't need all the information (e.g. you just want to get the IDs of the items a user has in their favorites list.)
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnOnlyIDs - Return only the IDs of items?
	 * @return bool - true upon success. false if the UGC query handle is invalid or if the UGC query handle is from CreateQueryUGCDetailsRequest.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnOnlyIDs(FUGCQueryHandle handle, bool bReturnOnlyIDs) const { return SteamUGC()->SetReturnOnlyIDs(handle, bReturnOnlyIDs); }

	/**
	 * Sets whether to return the the playtime stats on a pending UGC Query.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param int32 Days - The number of days worth of playtime stats to return.
	 * @return bool - true upon success. false if the UGC query handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnPlaytimeStats(FUGCQueryHandle handle, int32 Days) const { return SteamUGC()->SetReturnPlaytimeStats(handle, Days); }

	/**
	 * Sets whether to only return the the total number of matching items on a pending UGC Query.
	 * The actual items will not be returned when SteamUGCQueryCompleted_t is called.
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param bool bReturnTotalOnly - Only return the total number of items?
	 * @return bool - true upon success. false if the UGC query handle is invalid or if the UGC query handle is from CreateQueryUGCDetailsRequest
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetReturnTotalOnly(FUGCQueryHandle handle, bool bReturnTotalOnly) const { return SteamUGC()->SetReturnTotalOnly(handle, bReturnTotalOnly); }

	/**
	 * Sets a string to that items need to match in either the title or the description on a pending UGC Query.
	 * NOTE: This can only be used with CreateQueryAllUGCRequest!
	 * NOTE: This must be set before you send a UGC Query handle using SendQueryUGCRequest.
	 *
	 * @param FUGCQueryHandle handle - The UGC query handle to customize.
	 * @param const FString & SearchText - The text to be searched for.
	 * @return bool - true upon success. false if the UGC query handle is invalid, if the UGC query handle is not from CreateQueryAllUGCRequest or if pSearchText is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool SetSearchText(FUGCQueryHandle handle, const FString& SearchText) const { return SteamUGC()->SetSearchText(handle, TCHAR_TO_UTF8(*SearchText)); }

	/**
	 * Allows the user to rate a workshop item up or down.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item ID to vote on.
	 * @param bool bVoteUp - Vote up (true) or down (false)?
	 * @return FSteamAPICall - SteamAPICall_t to be used with a SetUserItemVoteResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall SetUserItemVote(FPublishedFileId PublishedFileID, bool bVoteUp) const { return SteamUGC()->SetUserItemVote(PublishedFileID, bVoteUp); }

	/**
	 * Starts the item update process.
	 * This gets you a handle that you can use to modify the item before finally sending off the update to the server with SubmitItemUpdate.
	 *
	 * @param int32 ConsumerAppId - The App ID that will be using this item.
	 * @param FPublishedFileId PublishedFileID - The item to update.
	 * @return FUGCUpdateHandle - A handle that you can use with future calls to modify the item before finally sending the update.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FUGCUpdateHandle StartItemUpdate(int32 ConsumerAppId, FPublishedFileId PublishedFileID) const { return SteamUGC()->StartItemUpdate(ConsumerAppId, PublishedFileID); }

	// #NOTE: These methods need to be async
	// #TODO: StartPlaytimeTracking
	// #TODO: StopPlaytimeTracking
	// #TODO: StopPlaytimeTrackingForAllItems

	/**
	 * Uploads the changes made to an item to the Steam Workshop.
	 * You can track the progress of an item update with GetItemUpdateProgress.
	 *
	 * @param FUGCUpdateHandle handle - The update handle to submit.
	 * @param const FString & ChangeNote - A brief description of the changes made. (Optional, set to NULL for no change note)
	 * @return FSteamAPICall - SteamAPICall_t to be used with a SubmitItemUpdateResult_t call result. Returns k_uAPICallInvalid if handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall SubmitItemUpdate(FUGCUpdateHandle handle, const FString& ChangeNote) const { return SteamUGC()->SubmitItemUpdate(handle, TCHAR_TO_UTF8(*ChangeNote)); }

	/**
	 * Subscribe to a workshop item. It will be downloaded and installed as soon as possible.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to subscribe to.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a RemoteStorageSubscribePublishedFileResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall SubscribeItem(FPublishedFileId PublishedFileID) const { return SteamUGC()->SubscribeItem(PublishedFileID); }

	/**
	 * Suspends and resumes all workshop downloads.
	 * If you call this with bSuspend set to true then downloads will be suspended until you resume them by setting bSuspend to false or when the game ends.
	 *
	 * @param bool bSuspend - Suspend (true) or Resume (false) workshop downloads?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|UGC")
	void SuspendDownloads(bool bSuspend) { SteamUGC()->SuspendDownloads(bSuspend); }

	/**
	 * Unsubscribe from a workshop item. This will result in the item being removed after the game quits.
	 *
	 * @param FPublishedFileId PublishedFileID - The workshop item to unsubscribe from.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a RemoteStorageUnsubscribePublishedFileResult_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	FSteamAPICall UnsubscribeItem(FPublishedFileId PublishedFileID) const { return SteamUGC()->UnsubscribeItem(PublishedFileID); }

	/**
	 * Updates an existing additional preview file for the item.
	 * If the preview type is an image then the format should be one that both the web and the application (if necessary) can render, and must be under 1MB. Suggested formats include JPG, PNG and GIF.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param int32 index - The index of the preview file from 0 to GetQueryUGCNumAdditionalPreviews.
	 * @param const FString & PreviewFile - Absolute path to the local image.
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool UpdateItemPreviewFile(FUGCUpdateHandle handle, int32 index, const FString& PreviewFile) const { return SteamUGC()->UpdateItemPreviewFile(handle, index, TCHAR_TO_UTF8(*PreviewFile)); }

	/**
	 * Updates an additional video preview from YouTube for the item.
	 * NOTE: This must be set before you submit the UGC update handle using SubmitItemUpdate.
	 *
	 * @param FUGCUpdateHandle handle - The workshop item update handle to customize.
	 * @param int32 index - The index of the preview file from 0 to GetQueryUGCNumAdditionalPreviews.
	 * @param const FString & VideoID - The YouTube video to add. (e.g. "jHgZh4GV9G0")
	 * @return bool - true upon success. false if the UGC update handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|UGC")
	bool UpdateItemPreviewVideo(FUGCUpdateHandle handle, int32 index, const FString& VideoID) const { return SteamUGC()->UpdateItemPreviewVideo(handle, index, TCHAR_TO_UTF8(*VideoID)); }

	/** Delegates */

	/** The result of a call to AddAppDependency. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnAddAppDependencyResult"))
	FOnAddAppDependencyResultDelegate OnAddAppDependencyResultDelegate;

	/** The result of a call to AddDependency. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnAddUGCDependencyResult"))
	FOnAddUGCDependencyResultDelegate OnAddUGCDependencyResultDelegate;

	/** Called when a new workshop item has been created. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnCreateItemResult"))
	FOnCreateItemResultDelegate OnCreateItemResultDelegate;

	/** Called when a workshop item has been downloaded. NOTE: This callback goes out to all running applications, ensure that the app ID associated with the item matches what you expect. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnDownloadItemResult"))
	FOnDownloadItemResultDelegate OnDownloadItemResultDelegate;

	/** Called when getting the app dependencies for an item. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnGetAppDependenciesResult"))
	FOnGetAppDependenciesResultDelegate OnGetAppDependenciesResultDelegate;

	/** Called when an attempt at deleting an item completes. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnDeleteItemResult"))
	FOnDeleteItemResultDelegate OnDeleteItemResultDelegate;

	/** Called when getting the users vote status on an item. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnGetUserItemVoteResult"))
	FOnGetUserItemVoteResultDelegate OnGetUserItemVoteResultDelegate;

	/** Called when a workshop item has been installed or updated. NOTE: This callback goes out to all running applications, ensure that the app ID associated with the item matches what you expect. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnItemInstalled"))
	FOnItemInstalledDelegate OnItemInstalledDelegate;

	/** Purpose: The result of a call to RemoveAppDependency. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnRemoveAppDependencyResult"))
	FOnRemoveAppDependencyResultDelegate OnRemoveAppDependencyResultDelegate;

	/** Purpose: The result of a call to RemoveDependency. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnRemoveUGCDependencyResult"))
	FOnRemoveUGCDependencyResultDelegate OnRemoveUGCDependencyResultDelegate;

	/** Called when the user has voted on an item. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnSetUserItemVoteResult"))
	FOnSetUserItemVoteResultDelegate OnSetUserItemVoteResultDelegate;

	/** Called when workshop item playtime tracking has started. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnStartPlaytimeTrackingResult"))
	FOnStartPlaytimeTrackingResultDelegate OnStartPlaytimeTrackingResultDelegate;

	/** Called when a UGC query request completes. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnSteamUGCQueryCompleted"))
	FOnSteamUGCQueryCompletedDelegate OnSteamUGCQueryCompletedDelegate;

	/** Called when workshop item playtime tracking has stopped. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnStopPlaytimeTrackingResult"))
	FOnStopPlaytimeTrackingResultDelegate OnStopPlaytimeTrackingResultDelegate;

	/** Called when an item update has completed. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnSubmitItemUpdateResult"))
	FOnSubmitItemUpdateResultDelegate OnSubmitItemUpdateResultDelegate;

	/** Called when the user has added or removed an item to/from their favorites. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|UGC", meta = (DisplayName = "OnUserFavoriteItemsListChanged"))
	FOnUserFavoriteItemsListChangedDelegate OnUserFavoriteItemsListChangedDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamUGC, OnAddAppDependencyResult, AddAppDependencyResult_t, OnAddAppDependencyResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnAddUGCDependencyResult, AddUGCDependencyResult_t, OnAddUGCDependencyResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnCreateItemResult, CreateItemResult_t, OnCreateItemResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnDownloadItemResult, DownloadItemResult_t, OnDownloadItemResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnGetAppDependenciesResult, GetAppDependenciesResult_t, OnGetAppDependenciesResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnDeleteItemResult, DeleteItemResult_t, OnDeleteItemResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnGetUserItemVoteResult, GetUserItemVoteResult_t, OnGetUserItemVoteResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnItemInstalled, ItemInstalled_t, OnItemInstalledCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnRemoveAppDependencyResult, RemoveAppDependencyResult_t, OnRemoveAppDependencyResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnRemoveUGCDependencyResult, RemoveUGCDependencyResult_t, OnRemoveUGCDependencyResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnSetUserItemVoteResult, SetUserItemVoteResult_t, OnSetUserItemVoteResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnStartPlaytimeTrackingResult, StartPlaytimeTrackingResult_t, OnStartPlaytimeTrackingResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnSteamUGCQueryCompleted, SteamUGCQueryCompleted_t, OnSteamUGCQueryCompletedCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnStopPlaytimeTrackingResult, StopPlaytimeTrackingResult_t, OnStopPlaytimeTrackingResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnSubmitItemUpdateResult, SubmitItemUpdateResult_t, OnSubmitItemUpdateResultCallback);
	STEAM_CALLBACK_MANUAL(USteamUGC, OnUserFavoriteItemsListChanged, UserFavoriteItemsListChanged_t, OnUserFavoriteItemsListChangedCallback);
};

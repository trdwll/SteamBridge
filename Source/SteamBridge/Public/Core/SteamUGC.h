// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamUGC.generated.h"

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

	// #TODO: AddRequiredTagGroup

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
	 * @param int32 & TimeStamp - Returns the time when the workshop item was last updated.
	 * @return bool - true if the workshop item is already installed. false in the following cases:
	 * cchFolderSize is 0.
	 * The workshop item has no content.
	 * The workshop item is not installed.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|UGC")
	bool GetItemInstallInfo(FPublishedFileId PublishedFileID, int64& SizeOnDisk, FString& FolderName, int32 FolderSize, int32& TimeStamp) const;

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































protected:
private:
};

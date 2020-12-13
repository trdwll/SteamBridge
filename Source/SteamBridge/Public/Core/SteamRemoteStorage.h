// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamRemoteStorage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnRemoteStorageDownloadUGCResultDelegate, ESteamResult, Result, FUGCHandle, FileHandle, int32, AppID, int32, SizeInBytes, FString, FileName, FSteamID, SteamIDOwner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRemoteStorageFileReadAsyncCompleteDelegate, FSteamAPICall, FileReadAsyncHandle, ESteamResult, Result, int32, Offset, int32, BytesRead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoteStorageFileShareResultDelegate, ESteamResult, Result, FUGCHandle, FileHandle, FString, FileName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoteStorageFileWriteAsyncCompleteDelegate, ESteamResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoteStoragePublishedFileSubscribedDelegate, FPublishedFileId, PublishedFileID, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoteStoragePublishedFileUnsubscribedDelegate, FPublishedFileId, PublishedFileID, int32, AppID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoteStorageSubscribePublishedFileResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoteStorageUnsubscribePublishedFileResultDelegate, ESteamResult, Result, FPublishedFileId, PublishedFileID);

/**
 * Provides functions for reading, writing, and accessing files which can be stored remotely in the Steam Cloud.
 * https://partner.steamgames.com/doc/api/ISteamRemoteStorage
 */
UCLASS()
class STEAMBRIDGE_API USteamRemoteStorage final : public UObject
{
	GENERATED_BODY()

public:
	USteamRemoteStorage();
	~USteamRemoteStorage();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Remote Storage", CompactNodeTitle = "SteamRemoteStorage"))
	static USteamRemoteStorage* GetSteamRemoteStorage() { return USteamRemoteStorage::StaticClass()->GetDefaultObject<USteamRemoteStorage>(); }

	/**
	 * Deletes a file from the local disk, and propagates that delete to the cloud.
	 * This is meant to be used when a user actively deletes a file. Use FileForget if you want to remove a file from the Steam Cloud but retain it on the users local disk.
	 * When a file has been deleted it can be re-written with FileWrite to reupload it to the Steam Cloud.
	 *
	 * @param const FString & FileName - The name of the file that will be deleted.
	 * @return bool - true if the file exists and has been successfully deleted; otherwise, false if the file did not exist.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileDelete(const FString& FileName) const { return SteamRemoteStorage()->FileDelete(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Checks whether the specified file exists.
	 *
	 * @param const FString & FileName - The name of the file.
	 * @return bool - true if the file exists; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	bool FileExists(const FString& FileName) const { return SteamRemoteStorage()->FileExists(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Deletes the file from remote storage, but leaves it on the local disk and remains accessible from the API.
	 * When you are out of Cloud space, this can be used to allow calls to FileWrite to keep working without needing to make the user delete files.
	 * How you decide which files to forget are up to you. It could be a simple Least Recently Used (LRU) queue or something more complicated.
	 * Requiring the user to manage their Cloud-ized files for a game, while is possible to do, it is never recommended. For instance, "Which file would you like to delete so that you may store this new one?" removes a
	 * significant advantage of using the Cloud in the first place: its transparency.
	 * Once a file has been deleted or forgotten, calling FileWrite will resynchronize it in the Cloud. Rewriting a forgotten file is the only way to make it persisted again.
	 *
	 * @param const FString & FileName - The name of the file that will be forgotten.
	 * @return bool - true if the file exists and has been successfully forgotten; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileForget(const FString& FileName) const { return SteamRemoteStorage()->FileForget(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Checks if a specific file is persisted in the steam cloud.
	 *
	 * @param const FString & FileName - The name of the file.
	 * @return bool - true if the file exists and the file is persisted in the Steam Cloud. false if FileForget was called on it and is only available locally.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	bool FilePersisted(const FString& FileName) const { return SteamRemoteStorage()->FilePersisted(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Opens a binary file, reads the contents of the file into a byte array, and then closes the file.
	 * NOTE: This is a synchronous call and as such is a will block your calling thread on the disk IO, and will also block the SteamAPI, which can cause other threads in your application to block. To avoid "hitching"
	 * due to a busy disk on the client machine using FileReadAsync, the asynchronous version of this API is recommended.
	 *
	 * @param const FString & FileName - The name of the file to read from.
	 * @param TArray<uint8> & Data - The buffer that the file will be read into. This buffer must be at least the same size provided to cubDataToRead.
	 * @param int32 DataToRead - The amount of bytes to read. Generally obtained from GetFileSize or GetFileTimestamp.
	 * @return int32 - The number of bytes read. Returns 0 if the file doesn't exist or the read fails.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	int32 FileRead(const FString& FileName, TArray<uint8>& Data, int32 DataToRead) const;

	// #TODO: FileReadAsync
	// #TODO: FileReadAsyncComplete

	/**
	 * SteamAPICall_t to be used with a RemoteStorageFileShareResult_t call result.
	 *
	 * @param const FString & FileName - The file
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	FSteamAPICall FileShare(const FString& FileName) const { return SteamRemoteStorage()->FileShare(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Creates a new file, writes the bytes to the file, and then closes the file. If the target file already exists, it is overwritten.
	 * NOTE: This is a synchronous call and as such is a will block your calling thread on the disk IO, and will also block the SteamAPI, which can cause other threads in your application to block. To avoid "hitching"
	 * due to a busy disk on the client machine using FileWriteAsync, the asynchronous version of this API is recommended.
	 *
	 * @param const FString & FileName - The name of the file to write to.
	 * @param const TArray<uint8> & Data - The bytes to write to the file.
	 * @return bool - true if the write was successful.
	 * Otherwise, false under the following conditions:
	 * 	The file you're trying to write is larger than 100MiB as defined by k_unMaxCloudFileChunkSize.
	 * cubData is less than 0.
	 * pvData is NULL.
	 * You tried to write to an invalid path or filename. Because Steam Cloud is cross platform the files need to have valid names on all supported OSes and file systems. See Microsoft's documentation on Naming Files, Paths, and Namespaces.
	 * The current user's Steam Cloud storage quota has been exceeded. They may have run out of space, or have too many files.
	 * Steam could not write to the disk, the location might be read-only.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileWrite(const FString& FileName, const TArray<uint8>& Data) const { return SteamRemoteStorage()->FileWrite(TCHAR_TO_UTF8(*FileName), Data.GetData(), Data.Num()); }

	// #TODO: FileWriteAsync

	/**
	 * Cancels a file write stream that was started by FileWriteStreamOpen. This trashes all of the data written and closes the write stream, but if there was an existing file with this name, it remains untouched.
	 *
	 * @param FUGCFileWriteStreamHandle WriteHandle - The file write stream to cancel.
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileWriteStreamCancel(FUGCFileWriteStreamHandle WriteHandle) const { return SteamRemoteStorage()->FileWriteStreamCancel(WriteHandle); }

	/**
	 * Closes a file write stream that was started by FileWriteStreamOpen. This flushes the stream to the disk, overwriting the existing file if there was one.
	 *
	 * @param FUGCFileWriteStreamHandle WriteHandle - The file write stream to close.
	 * @return bool - true if the file write stream was successfully closed, the file has been committed to the disk. false if writeHandle is not a valid file write stream.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileWriteStreamClose(FUGCFileWriteStreamHandle WriteHandle) const { return SteamRemoteStorage()->FileWriteStreamClose(WriteHandle); }

	/**
	 * Creates a new file output stream allowing you to stream out data to the Steam Cloud file in chunks. If the target file already exists, it is not overwritten until FileWriteStreamClose has been called.
	 * To write data out to this stream you can use FileWriteStreamWriteChunk, and then to close or cancel you use FileWriteStreamClose and FileWriteStreamCancel respectively.
	 *
	 * @param const FString & FileName - The name of the file to write to.
	 * @return FUGCFileWriteStreamHandle - Returns k_UGCFileStreamHandleInvalid under the following conditions:
	 * You tried to write to an invalid path or filename. Because Steam Cloud is cross platform the files need to have valid names on all supported OSes and file systems. See Microsoft's documentation on Naming Files, Paths, and Namespaces.
	 * The current user's Steam Cloud storage quota has been exceeded. They may have run out of space, or have too many files.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	FUGCFileWriteStreamHandle FileWriteStreamOpen(const FString& FileName) const { return SteamRemoteStorage()->FileWriteStreamOpen(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Writes a blob of data to the file write stream.
	 *
	 * @param FUGCFileWriteStreamHandle WriteHandle - The file write stream to write to.
	 * @param const TArray<uint8> & Data - The data to write to the stream.
	 * @return bool - true if the data was successfully written to the file write stream.
	 * false if writeHandle is not a valid file write stream, cubData is negative or larger than k_unMaxCloudFileChunkSize, or the current user's Steam Cloud storage quota has been exceeded. They may have run out of space, or have too many files.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool FileWriteStreamWriteChunk(FUGCFileWriteStreamHandle WriteHandle, const TArray<uint8>& Data) const { return SteamRemoteStorage()->FileWriteStreamWriteChunk(WriteHandle, Data.GetData(), Data.Num()); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	int32 GetCachedUGCCount() const { return SteamRemoteStorage()->GetCachedUGCCount(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	FUGCHandle GetCachedUGCHandle(int32 CachedContent) const { return SteamRemoteStorage()->GetCachedUGCHandle(CachedContent); }

	/**
	 * Gets the total number of local files synchronized by Steam Cloud. Used for enumeration with GetFileNameAndSize.
	 *
	 * @return int32 - The number of files present for the current user, including files in subfolders.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	int32 GetFileCount() const { return SteamRemoteStorage()->GetFileCount(); }

	/**
	 * Gets the file name and size of a file from the index.
	 * NOTE: You must call GetFileCount first to get the number of files.
	 *
	 * @param int32 FileIndex - The index of the file, this should be between 0 and GetFileCount.
	 * @param int32 & FileSizeInBytes - Returns the file size in bytes.
	 * @return FString - The name of the file at the specified index, if it exists. Returns an empty string ("") if the file doesn't exist.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	FString GetFileNameAndSize(int32 FileIndex, int32& FileSizeInBytes) const { return UTF8_TO_TCHAR(SteamRemoteStorage()->GetFileNameAndSize(FileIndex, &FileSizeInBytes)); }

	/**
	 * Gets the specified files size in bytes.
	 *
	 * @param const FString & FileName - The name of the file.
	 * @return int32 - The size of the file in bytes. Returns0 if the file does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	int32 GetFileSize(const FString& FileName) const { return SteamRemoteStorage()->GetFileSize(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Gets the specified file's last modified timestamp in Unix epoch format (seconds since Jan 1st 1970).
	 *
	 * @param const FString & FileName - The name of the file.
	 * @return int64 - The last modified timestamp in Unix epoch format (seconds since Jan 1st 1970).
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	int64 GetFileTimestamp(const FString& FileName) const { return SteamRemoteStorage()->GetFileTimestamp(TCHAR_TO_UTF8(*FileName)); }

	/**
	 * Gets the number of bytes available, and used on the users Steam Cloud storage.
	 *
	 * @param int64 & TotalBytes - Returns the total amount of bytes the user has access to.
	 * @param int64 & AvailableBytes - Returns the number of bytes available.
	 * @return bool - This function always returns true.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool GetQuota(int64& TotalBytes, int64& AvailableBytes) const;

	/**
	 * Obtains the platforms that the specified file will syncronize to.
	 *
	 * @param const FString & FileName - The name of the file.
	 * @return ESteamRemoteStoragePlatform - Bitfield containing the platforms that the file was set to with SetSyncPlatforms.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	ESteamRemoteStoragePlatform GetSyncPlatforms(const FString& FileName) const { return (ESteamRemoteStoragePlatform)SteamRemoteStorage()->GetSyncPlatforms(TCHAR_TO_UTF8(*FileName)); }

	// #NOTE: No docs for this method currently
	// #TODO: GetUGCDetails

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	bool GetUGCDownloadProgress(FUGCHandle ContentHandle, int32& BytesDownloaded, int32& BytesExpected) const { return SteamRemoteStorage()->GetUGCDownloadProgress(ContentHandle, &BytesDownloaded, &BytesExpected); }

	/**
	 * Checks if the account wide Steam Cloud setting is enabled for this user; or if they disabled it in the Settings->Cloud dialog.
	 * Ensure that you are also checking IsCloudEnabledForApp, as these two options are mutually exclusive.
	 *
	 * @return bool - true if Steam Cloud is enabled for this account; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	bool IsCloudEnabledForAccount() const { return SteamRemoteStorage()->IsCloudEnabledForAccount(); }

	/**
	 * Checks if the per game Steam Cloud setting is enabled for this user; or if they disabled it in the Game Properties->Update dialog.
	 * Ensure that you are also checking IsCloudEnabledForAccount, as these two options are mutually exclusive.
	 * It's generally recommended that you allow the user to toggle this setting within your in-game options, you can toggle it with SetCloudEnabledForApp.
	 *
	 * @return bool - true if Steam Cloud is enabled for this app; otherwise, false.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|RemoteStorage")
	bool IsCloudEnabledForApp() const { return SteamRemoteStorage()->IsCloudEnabledForApp(); }

	/**
	 * Toggles whether the Steam Cloud is enabled for your application. This setting can be queried with IsCloudEnabledForApp.
	 * NOTE: This must only ever be called as the direct result of the user explicitly requesting that it's enabled or not. This is typically accomplished with a checkbox within your in-game options.
	 *
	 * @param bool bEnabled - Enable (true) or disable (false) the Steam Cloud for this application?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|RemoteStorage")
	void SetCloudEnabledForApp(bool bEnabled) const { SteamRemoteStorage()->SetCloudEnabledForApp(bEnabled); }

	/**
	 * Allows you to specify which operating systems a file will be synchronized to.
	 * Use this if you have a multiplatform game but have data which is incompatible between platforms.
	 * Files default to k_ERemoteStoragePlatformAll when they are first created. You can use the bitwise OR operator, "|" to specify multiple platforms.
	 *
	 * @param const FString & FileName - The name of the file.
	 * @param ESteamRemoteStoragePlatform RemoteStoragePlatform - 	The platforms that the file will be syncronized to.
	 * @return bool - true if the file exists, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	bool SetSyncPlatforms(const FString& FileName, ESteamRemoteStoragePlatform RemoteStoragePlatform) const { return SteamRemoteStorage()->SetSyncPlatforms(TCHAR_TO_UTF8(*FileName), (ERemoteStoragePlatform)RemoteStoragePlatform); }

	/**
	 * SteamAPICall_t to be used with a RemoteStorageDownloadUGCResult_t call result.
	 *
	 * @param FUGCHandle ContentHandle
	 * @param int32 Priority
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	FSteamAPICall UGCDownload(FUGCHandle ContentHandle, int32 Priority) const { return SteamRemoteStorage()->UGCDownload(ContentHandle, Priority); }

	/**
	 * SteamAPICall_t to be used with a RemoteStorageDownloadUGCResult_t call result.
	 *
	 * @param FUGCHandle ContentHandle
	 * @param const FString & Location
	 * @param int32 Priority
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|RemoteStorage")
	FSteamAPICall UGCDownloadToLocation(FUGCHandle ContentHandle, const FString& Location, int32 Priority) const { return SteamRemoteStorage()->UGCDownloadToLocation(ContentHandle, TCHAR_TO_UTF8(*Location), Priority); }

	// #NOTE: No docs for this method currently
	// #TODO: UGCRead

	/** Delegates */

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageDownloadUGCResult"))
	FOnRemoteStorageDownloadUGCResultDelegate m_OnRemoteStorageDownloadUGCResult;

	/** Response when reading a file asyncrounously with FileReadAsync. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageFileReadAsyncComplete"))
	FOnRemoteStorageFileReadAsyncCompleteDelegate m_OnRemoteStorageFileReadAsyncComplete;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageFileShareResult"))
	FOnRemoteStorageFileShareResultDelegate m_OnRemoteStorageFileShareResult;

	/** Response when writing a file asyncrounously with FileWriteAsync. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageFileWriteAsyncComplete"))
	FOnRemoteStorageFileWriteAsyncCompleteDelegate m_OnRemoteStorageFileWriteAsyncComplete;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStoragePublishedFileSubscribed"))
	FOnRemoteStoragePublishedFileSubscribedDelegate m_OnRemoteStoragePublishedFileSubscribed;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStoragePublishedFileUnsubscribed"))
	FOnRemoteStoragePublishedFileUnsubscribedDelegate m_OnRemoteStoragePublishedFileUnsubscribed;

	/** Called when the user has subscribed to a piece of UGC. Result from ISteamUGC::SubscribeItem. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageSubscribePublishedFileResult"))
	FOnRemoteStorageSubscribePublishedFileResultDelegate m_OnRemoteStorageSubscribePublishedFileResult;

	/** Called when the user has unsubscribed from a piece of UGC. Result from ISteamUGC::UnsubscribeItem. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|RemoteStorage", meta = (DisplayName = "OnRemoteStorageUnsubscribePublishedFileResult"))
	FOnRemoteStorageUnsubscribePublishedFileResultDelegate m_OnRemoteStorageUnsubscribePublishedFileResult;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageDownloadUGCResult, RemoteStorageDownloadUGCResult_t, OnRemoteStorageDownloadUGCResultCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageFileReadAsyncComplete, RemoteStorageFileReadAsyncComplete_t, OnRemoteStorageFileReadAsyncCompleteCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageFileShareResult, RemoteStorageFileShareResult_t, OnRemoteStorageFileShareResultCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageFileWriteAsyncComplete, RemoteStorageFileWriteAsyncComplete_t, OnRemoteStorageFileWriteAsyncCompleteCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStoragePublishedFileSubscribed, RemoteStoragePublishedFileSubscribed_t, OnRemoteStoragePublishedFileSubscribedCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStoragePublishedFileUnsubscribed, RemoteStoragePublishedFileUnsubscribed_t, OnRemoteStoragePublishedFileUnsubscribedCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageSubscribePublishedFileResult, RemoteStorageSubscribePublishedFileResult_t, OnRemoteStorageSubscribePublishedFileResultCallback);
	STEAM_CALLBACK_MANUAL(USteamRemoteStorage, OnRemoteStorageUnsubscribePublishedFileResult, RemoteStorageUnsubscribePublishedFileResult_t, OnRemoteStorageUnsubscribePublishedFileResultCallback);
};

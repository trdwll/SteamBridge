// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamHTTP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHTTPRequestCompletedDelegate, FHTTPRequestHandle, RequestHandle, int64, ContextValue, bool, bRequestSuccessful, ESteamHTTPStatus::Type, HTTPStatus, int32, BodySize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHTTPRequestDataReceivedDelegate, FHTTPRequestHandle, RequestHandle, int64, ContextValue, int32, Offset, int32, BytesReceived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTTPRequestHeadersReceivedDelegate, FHTTPRequestHandle, RequestHandle, int64, ContextValue);

/**
 * A small and easy to use HTTP client to send and receive data from the web.
 * https://partner.steamgames.com/doc/api/ISteamHTTP
 */
UCLASS()
class STEAMBRIDGE_API USteamHTTP final : public UObject
{
	GENERATED_BODY()

public:
	USteamHTTP();
	~USteamHTTP();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "Steam HTTP", CompactNodeTitle = "SteamHTTP"))
	static USteamHTTP* GetSteamHTTP() { return USteamHTTP::StaticClass()->GetDefaultObject<USteamHTTP>(); }

	/**
	 * Creates a cookie container to store cookies during the lifetime of the process.
	 * This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
	 * If bAllowResponsesToModify is true then any response to your requests using this cookie container may add new cookies to the container which may be transmitted with future requests. Otherwise, if it's -
	 * false then only cookies you explicitly set will be sent.
	 * You can associate the cookie container with a http request by using SetHTTPRequestCookieContainer, and you can set a cookie using SetCookie.
	 * Don't forget to free the container when you're done with it to prevent leaking memory by calling ReleaseCookieContainer!
	 *
	 * @param bool bAllowResponsesToModify
	 * @return FHTTPCookieContainerHandle
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	FHTTPCookieContainerHandle CreateCookieContainer(bool bAllowResponsesToModify) { return SteamHTTP()->CreateCookieContainer(bAllowResponsesToModify); }

	/**
	 * Initializes a new HTTP request.
	 * Requires the method such as GET or POST and the absolute URL for the request. Both http and https are supported, so this string must start with "http://" or "https://" and should look like -
	 * "http://store.steampowered.com/app/10/" or similar. This call returns a handle that you can use to make further calls to setup and then send the HTTP request with SendHTTPRequest or SendHTTPRequestAndStreamResponse.
	 * Don't forget to free the HTTP request when you're done with it to prevent leaking memory by calling ReleaseHTTPRequest!
	 *
	 * @param ESteamHTTPMethod HTTPRequestMethod
	 * @param const FString & AbsoluteURL
	 * @return FHTTPRequestHandle
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	FHTTPRequestHandle CreateHTTPRequest(ESteamHTTPMethod HTTPRequestMethod, const FString& AbsoluteURL) { return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)HTTPRequestMethod, TCHAR_TO_UTF8(*AbsoluteURL)); }

	/**
	 * Defers a request which has already been sent by moving it at the back of the queue.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool DeferHTTPRequest(FHTTPRequestHandle RequestHandle) { return SteamHTTP()->DeferHTTPRequest(RequestHandle); }

	/**
	 * Gets progress on downloading the body for the request.
	 * This will be zero unless a response header has already been received which included a content-length field. For responses that contain no content-length it will report zero for the duration of the request  -
	 * as the size is unknown until the connection closes.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param float & Percent
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPDownloadProgressPct(FHTTPRequestHandle RequestHandle, float& Percent) { return SteamHTTP()->GetHTTPDownloadProgressPct(RequestHandle, &Percent); }

	/**
	 * Check if the reason the request failed was because we timed it out (rather than some harder failure).
	 * You'll want to call this within the context of HTTPRequestCompleted_t if m_bRequestSuccessful is false.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param bool & bWasTimedOut
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPRequestWasTimedOut(FHTTPRequestHandle RequestHandle, bool& bWasTimedOut) { return SteamHTTP()->GetHTTPRequestWasTimedOut(RequestHandle, &bWasTimedOut); }

	/**
	 * Gets the body data from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. You should first call  -
	 * GetHTTPResponseBodySize or use the m_unBodySize variable provided in the call result, you can then allocate a buffer with that size to pass into this function.
	 * This is only for HTTP requests which were sent with SendHTTPRequest. Use GetHTTPStreamingResponseBodyData if you're using streaming HTTP requests via SendHTTPRequestAndStreamResponse.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param uint8 & BodyDataBuffer
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodyData(FHTTPRequestHandle RequestHandle, uint8& BodyDataBuffer) { return SteamHTTP()->GetHTTPResponseBodyData(RequestHandle, &BodyDataBuffer, MAX_uint32); }

	/**
	 * Gets the size of the body data from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t or HTTPRequestDataReceived_t associated with this request handle. If this returns -
	 * successfully, then you can allocate a buffer with the provided size to fill with the data obtained from GetHTTPResponseBodyData or GetHTTPStreamingResponseBodyData.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param int32 & BodySize
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodySize(FHTTPRequestHandle RequestHandle, int32& BodySize);

	/**
	 * Checks if a header is present in an HTTP response and returns its size.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. If the response header exists in the -
	 * response, then you can allocate a correctly sized buffer to get the associated value with GetHTTPResponseHeaderValue.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & HeaderName
	 * @param int32 & ResponseHeaderSize
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderSize(FHTTPRequestHandle RequestHandle, const FString& HeaderName, int32& ResponseHeaderSize);

	/**
	 * Gets a header value from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. You should first call  -
	 * GetHTTPResponseHeaderSize to check for the presence of the header and to get the size. You can then allocate a buffer with that size and pass it into this function.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & HeaderName
	 * @param uint8 & HeaderValueBuffer
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderValue(FHTTPRequestHandle RequestHandle, const FString& HeaderName, uint8& HeaderValueBuffer) { return SteamHTTP()->GetHTTPResponseHeaderValue(RequestHandle, TCHAR_TO_UTF8(*HeaderName), &HeaderValueBuffer, MAX_uint32); }

	/**
	 * Gets the body data from a streaming HTTP response.
	 * This must be called after data is received from a streaming HTTP request via the HTTPRequestDataReceived_t callback associated with this request handle. Typically you'll want to allocate a buffer associated -
	 * with the request handle using the Content-Length HTTP response field to receive the total size of the data when you receive the header via HTTPRequestHeadersReceived_t. You can then append data to that buffer as it comes in.
	 * This is only for streaming HTTP requests which were sent with SendHTTPRequestAndStreamResponse. Use GetHTTPResponseBodyData if you're using SendHTTPRequest.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param int32 Offset
	 * @param uint8 & BodyDataBuffer
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPStreamingResponseBodyData(FHTTPRequestHandle RequestHandle, int32 Offset, uint8& BodyDataBuffer) { return SteamHTTP()->GetHTTPStreamingResponseBodyData(RequestHandle, Offset, &BodyDataBuffer, MAX_uint32); }

	/**
	 * Prioritizes a request which has already been sent by moving it at the front of the queue.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool PrioritizeHTTPRequest(FHTTPRequestHandle RequestHandle) { return SteamHTTP()->PrioritizeHTTPRequest(RequestHandle); }

	/**
	 * Releases a cookie container, freeing the memory allocated within Steam.
	 * You MUST call this when you are done using each HTTPCookieContainerHandle that you obtained via CreateCookieContainer!
	 *
	 * @param FHTTPCookieContainerHandle CookieContainerHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool ReleaseCookieContainer(FHTTPCookieContainerHandle CookieContainerHandle) { return SteamHTTP()->ReleaseCookieContainer(CookieContainerHandle); }

	/**
	 * Releases an HTTP request handle, freeing the memory allocated within Steam.
	 * You MUST call this when you are done using each HTTPRequestHandle that you obtained via CreateHTTPRequest!
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool ReleaseHTTPRequest(FHTTPRequestHandle RequestHandle) { return SteamHTTP()->ReleaseHTTPRequest(RequestHandle); }

	/**
	 * Sends an HTTP request.
	 * This call is asynchronous and provides a call result handle which you must use to track the call to its completion. If you have multiple requests in flight at the same time you can use PrioritizeHTTPRequest or DeferHTTPRequest to set the priority of the request.
	 * If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.
	 * If the data you are expecting is large, you can use SendHTTPRequestAndStreamResponse to stream the data in chunks.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param FSteamAPICall & CallHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequest(FHTTPRequestHandle RequestHandle, FSteamAPICall& CallHandle);

	/**
	 * Sends an HTTP request and streams the response back in chunks.
	 * This call is asynchronous and provides a call result handle which you must use to track the call to its completion. Typically you'll want to allocate a buffer associated with the request handle using the -
	 * Content-Length HTTP response field to receive the total size of the data when you receive the header via HTTPRequestHeadersReceived_t. You can then append data to that buffer as it comes in.
	 * If you have multiple requests in flight at the same time you can use PrioritizeHTTPRequest or DeferHTTPRequest to set the priority of the request.
	 * If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.
	 * If the data you are expecting is small (on the order of a few megabytes or less) then you'll likely want to use SendHTTPRequest.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param FSteamAPICall & CallHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequestAndStreamResponse(FHTTPRequestHandle RequestHandle, FSteamAPICall& CallHandle);

	/**
	 * Adds a cookie to the specified cookie container that will be used with future requests.
	 *
	 * @param FHTTPCookieContainerHandle CookieContainerHandle
	 * @param const FString & Host
	 * @param const FString & Url
	 * @param const FString & Cookie
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetCookie(FHTTPCookieContainerHandle CookieContainerHandle, const FString& Host, const FString& Url, const FString& Cookie) { return SteamHTTP()->SetCookie(CookieContainerHandle, TCHAR_TO_UTF8(*Host), TCHAR_TO_UTF8(*Url), TCHAR_TO_UTF8(*Cookie)); }

	/**
	 * Set an absolute timeout in milliseconds for the HTTP request.
	 * This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param int32 Milliseconds
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestAbsoluteTimeoutMS(FHTTPRequestHandle RequestHandle, int32 Milliseconds) { return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(RequestHandle, Milliseconds); }

	/**
	 * Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request.
	 * This is just so the caller can easily keep track of which callbacks go with which request data.
	 * Must be called before sending the request.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param int64 ContextValue
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestContextValue(FHTTPRequestHandle RequestHandle, int64 ContextValue) { return SteamHTTP()->SetHTTPRequestContextValue(RequestHandle, ContextValue); }

	/**
	 * Associates a cookie container to use for an HTTP request.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param FHTTPCookieContainerHandle CookieContainerHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestCookieContainer(FHTTPRequestHandle RequestHandle, FHTTPCookieContainerHandle CookieContainerHandle) { return SteamHTTP()->SetHTTPRequestCookieContainer(RequestHandle, CookieContainerHandle); }

	/**
	 * Set a GET or POST parameter value on the HTTP request.
	 * Must be called prior to sending the request.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & Name
	 * @param const FString & Value
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestGetOrPostParameter(FHTTPRequestHandle RequestHandle, const FString& Name, const FString& Value) { return SteamHTTP()->SetHTTPRequestGetOrPostParameter(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Set a request header value for the HTTP request.
	 * Must be called before sending the request.
	 * A full list of standard request fields are available here on wikipedia. The User-Agent field is explicitly disallowed as it gets overwritten when the request is sent.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & Name
	 * @param const FString & Value
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestHeaderValue(FHTTPRequestHandle RequestHandle, const FString& Name, const FString& Value) { return SteamHTTP()->SetHTTPRequestHeaderValue(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Set the timeout in seconds for the HTTP request.
	 * The default timeout is 60 seconds if you don't call this. This can get bumped everytime we get more data. Use SetHTTPRequestAbsoluteTimeoutMS if you need a strict maximum timeout.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param int32 TimeoutSeconds
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestNetworkActivityTimeout(FHTTPRequestHandle RequestHandle, int32 TimeoutSeconds) { return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(RequestHandle, TimeoutSeconds); }

	/**
	 * Sets the body for an HTTP Post request.
	 * Will fail and return false on a GET request, and will fail if POST params have already been set for the request. Setting this raw body makes it the only contents for the post, the pchContentType parameter will -
	 * set the "content-type" header for the request to inform the server how to interpret the body.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & ContentType
	 * @param uint8 & Body
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRawPostBody(FHTTPRequestHandle RequestHandle, const FString& ContentType, uint8& Body) { return SteamHTTP()->SetHTTPRequestRawPostBody(RequestHandle, TCHAR_TO_UTF8(*ContentType), &Body, MAX_uint8); }

	/**
	 * Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store.
	 * This currently only works Windows and macOS.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param bool bRequireVerifiedCertificate
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRequiresVerifiedCertificate(FHTTPRequestHandle RequestHandle, bool bRequireVerifiedCertificate) { return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(RequestHandle, bRequireVerifiedCertificate); }

	/**
	 * Set additional user agent info for a request.
	 * This doesn't clobber the normal user agent, it just adds the extra info on the end. Sending NULL or an empty string resets the user agent info to the default value.
	 *
	 * @param FHTTPRequestHandle RequestHandle
	 * @param const FString & UserAgentInfo
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestUserAgentInfo(FHTTPRequestHandle RequestHandle, const FString& UserAgentInfo) { return SteamHTTP()->SetHTTPRequestUserAgentInfo(RequestHandle, TCHAR_TO_UTF8(*UserAgentInfo)); }

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestCompleted"))
	FOnHTTPRequestCompletedDelegate m_OnHTTPRequestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestDataReceived"))
	FOnHTTPRequestDataReceivedDelegate m_OnHTTPRequestDataReceived;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestHeadersReceived"))
	FOnHTTPRequestHeadersReceivedDelegate m_OnHTTPRequestHeadersReceived;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestCompleted, HTTPRequestCompleted_t, OnHTTPRequestCompletedCallback);
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestDataReceived, HTTPRequestDataReceived_t, OnHTTPRequestDataReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestHeadersReceived, HTTPRequestHeadersReceived_t, OnHTTPRequestHeadersReceivedCallback);
};

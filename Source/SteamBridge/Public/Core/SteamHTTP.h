// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

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

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam HTTP", CompactNodeTitle = "SteamHTTP"))
	static USteamHTTP* GetSteamHTTP() { return USteamHTTP::StaticClass()->GetDefaultObject<USteamHTTP>(); }

	/**
	 * Creates a cookie container to store cookies during the lifetime of the process.
	 * This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
	 * If bAllowResponsesToModify is true then any response to your requests using this cookie container may add new cookies to the container which may be transmitted with future requests. Otherwise, if it's -
	 * false then only cookies you explicitly set will be sent.
	 * You can associate the cookie container with a http request by using SetHTTPRequestCookieContainer, and you can set a cookie using SetCookie.
	 * Don't forget to free the container when you're done with it to prevent leaking memory by calling ReleaseCookieContainer!
	 *
	 * @param bool bAllowResponsesToModify - Set whether the server can set cookies in this container.
	 * @return FHTTPCookieContainerHandle - Returns a new cookie container handle to be used with future calls to SteamHTTP functions.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	FHTTPCookieContainerHandle CreateCookieContainer(bool bAllowResponsesToModify) const { return SteamHTTP()->CreateCookieContainer(bAllowResponsesToModify); }

	/**
	 * Initializes a new HTTP request.
	 * Requires the method such as GET or POST and the absolute URL for the request. Both http and https are supported, so this string must start with "http://" or "https://" and should look like -
	 * "http://store.steampowered.com/app/10/" or similar. This call returns a handle that you can use to make further calls to setup and then send the HTTP request with SendHTTPRequest or SendHTTPRequestAndStreamResponse.
	 * Don't forget to free the HTTP request when you're done with it to prevent leaking memory by calling ReleaseHTTPRequest!
	 *
	 * @param ESteamHTTPMethod HTTPRequestMethod - The type of request to make with this request.
	 * @param const FString & AbsoluteURL - The url to request. Must start with "http://" or "https://".
	 * @return FHTTPRequestHandle - Returns a new request handle to be used with future calls to SteamHTTP functions. Returns INVALID_HTTPREQUEST_HANDLE if pchAbsoluteURL is NULL or empty ("").
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	FHTTPRequestHandle CreateHTTPRequest(ESteamHTTPMethod HTTPRequestMethod, const FString& AbsoluteURL) const { return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)HTTPRequestMethod, TCHAR_TO_UTF8(*AbsoluteURL)); }

	/**
	 * Defers a request which has already been sent by moving it at the back of the queue.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to defer.
	 * @return bool - Returns true if the request has been successfully defered. Otherwise false if hRequest is an invalid handle, or if the request has not been sent yet.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool DeferHTTPRequest(FHTTPRequestHandle RequestHandle) const { return SteamHTTP()->DeferHTTPRequest(RequestHandle); }

	/**
	 * Gets progress on downloading the body for the request.
	 * This will be zero unless a response header has already been received which included a content-length field. For responses that contain no content-length it will report zero for the duration of the request  -
	 * as the size is unknown until the connection closes.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to get the download percentage for.
	 * @param float & Percent - Returns the download percentage if the call was successful.
	 * @return bool - Returns true upon success if the download percentage was successfully returned. Otherwise, false if the handle is invalid or pflPercentOut is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPDownloadProgressPct(FHTTPRequestHandle RequestHandle, float& Percent) const { return SteamHTTP()->GetHTTPDownloadProgressPct(RequestHandle, &Percent); }

	/**
	 * Check if the reason the request failed was because we timed it out (rather than some harder failure).
	 * You'll want to call this within the context of HTTPRequestCompleted_t if m_bRequestSuccessful is false.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to check the failure reason for.
	 * @param bool & bWasTimedOut - Returns whether the request was timed out or not.
	 * @return bool - Returns true upon success if we successfully checked .
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPRequestWasTimedOut(FHTTPRequestHandle RequestHandle, bool& bWasTimedOut) const { return SteamHTTP()->GetHTTPRequestWasTimedOut(RequestHandle, &bWasTimedOut); }

	/**
	 * Gets the body data from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. You should first call  -
	 * GetHTTPResponseBodySize or use the m_unBodySize variable provided in the call result, you can then allocate a buffer with that size to pass into this function.
	 * This is only for HTTP requests which were sent with SendHTTPRequest. Use GetHTTPStreamingResponseBodyData if you're using streaming HTTP requests via SendHTTPRequestAndStreamResponse.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to get the response body data for.
	 * @param uint8 & BodyDataBuffer - The buffer where the data will be copied into.
	 * @return bool - Returns true upon success indicating that pBodyDataBuffer has been filled with the body data.
	 * Otherwise, returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 * The request is a streaming request.
	 * pBodyDataBuffer is NULL.
	 * unBufferSize is not the same size that was provided by GetHTTPResponseBodySize.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodyData(FHTTPRequestHandle RequestHandle, uint8& BodyDataBuffer) const { return SteamHTTP()->GetHTTPResponseBodyData(RequestHandle, &BodyDataBuffer, MAX_uint32); }

	/**
	 * Gets the size of the body data from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t or HTTPRequestDataReceived_t associated with this request handle. If this returns -
	 * successfully, then you can allocate a buffer with the provided size to fill with the data obtained from GetHTTPResponseBodyData or GetHTTPStreamingResponseBodyData.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to get the response body size for.
	 * @param int32 & BodySize - Returns the size of the response body.
	 * @return bool - Returns true upon success indicating that unBodySize has been filled with the size.
	 * Otherwise, returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 * unBodySize is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodySize(FHTTPRequestHandle RequestHandle, int32& BodySize) const { return SteamHTTP()->GetHTTPResponseBodySize(RequestHandle, (uint32*)&BodySize); }

	/**
	 * Checks if a header is present in an HTTP response and returns its size.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. If the response header exists in the -
	 * response, then you can allocate a correctly sized buffer to get the associated value with GetHTTPResponseHeaderValue.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to check for the response header name.
	 * @param const FString & HeaderName - The header name to check.
	 * @param int32 & ResponseHeaderSize - 	Returns the size of the response header, if it is present in the response.
	 * @return bool - Returns true if the header name is present in the response and unResponseHeaderSize has been filled with the size of the header value.
	 * Otherwise, returns false and sets unResponseHeaderSize to 0 under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 * pchHeaderName is NULL.
	 * unResponseHeaderSize is NULL.
	 * The header name is not present in the response.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderSize(FHTTPRequestHandle RequestHandle, const FString& HeaderName, int32& ResponseHeaderSize) const { return SteamHTTP()->GetHTTPResponseHeaderSize(RequestHandle, TCHAR_TO_UTF8(*HeaderName), (uint32*)&ResponseHeaderSize); }

	/**
	 * Gets a header value from an HTTP response.
	 * This must be called after the HTTP request has completed and returned the HTTP response via the HTTPRequestCompleted_t call result associated with this request handle. You should first call  -
	 * GetHTTPResponseHeaderSize to check for the presence of the header and to get the size. You can then allocate a buffer with that size and pass it into this function.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to get the response header value for.
	 * @param const FString & HeaderName - The header name to get the header value for.
	 * @param uint8 & HeaderValueBuffer - The buffer where the value will be copied into.
	 * @return bool - Returns true upon success indicating that pHeaderValueBuffer has been filled with the header value.
	 * Otherwise, returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 * pchHeaderName is NULL.
	 * pHeaderValueBuffer is NULL.
	 * The header name is not present in the response.
	 * unBufferSize is not large enough to hold the value.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderValue(FHTTPRequestHandle RequestHandle, const FString& HeaderName, uint8& HeaderValueBuffer) const { return SteamHTTP()->GetHTTPResponseHeaderValue(RequestHandle, TCHAR_TO_UTF8(*HeaderName), &HeaderValueBuffer, MAX_uint32); }

	/**
	 * Gets the body data from a streaming HTTP response.
	 * This must be called after data is received from a streaming HTTP request via the HTTPRequestDataReceived_t callback associated with this request handle. Typically you'll want to allocate a buffer associated -
	 * with the request handle using the Content-Length HTTP response field to receive the total size of the data when you receive the header via HTTPRequestHeadersReceived_t. You can then append data to that buffer as it comes in.
	 * This is only for streaming HTTP requests which were sent with SendHTTPRequestAndStreamResponse. Use GetHTTPResponseBodyData if you're using SendHTTPRequest.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to get the response body data for.
	 * @param int32 Offset - This must be the offset provided by HTTPRequestDataReceived_t.
	 * @param uint8 & BodyDataBuffer - Returns the data by copying it into this buffer.
	 * @return bool - Returns true upon success indicating that pBodyDataBuffer has been filled with the body data.
	 * Otherwise, returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has not been sent or has not completed.
	 * The request is not a streaming request.
	 * cOffset is not the same offset that was provided by HTTPRequestDataReceived_t.
	 * unBufferSize is not the same size that was provided by HTTPRequestDataReceived_t.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPStreamingResponseBodyData(FHTTPRequestHandle RequestHandle, int32 Offset, uint8& BodyDataBuffer) const { return SteamHTTP()->GetHTTPStreamingResponseBodyData(RequestHandle, Offset, &BodyDataBuffer, MAX_uint32); }

	/**
	 * Prioritizes a request which has already been sent by moving it at the front of the queue.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to prioritize.
	 * @return bool - Returns true if the request has been successfully prioritized. Otherwise false if hRequest is an invalid handle, or if the request has not been sent yet.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool PrioritizeHTTPRequest(FHTTPRequestHandle RequestHandle) const { return SteamHTTP()->PrioritizeHTTPRequest(RequestHandle); }

	/**
	 * Releases a cookie container, freeing the memory allocated within Steam.
	 * You MUST call this when you are done using each HTTPCookieContainerHandle that you obtained via CreateCookieContainer!
	 *
	 * @param FHTTPCookieContainerHandle CookieContainerHandle - The cookie container handle to release.
	 * @return bool - Returns true if the handle has been freed; otherwise, false if the handle was invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool ReleaseCookieContainer(FHTTPCookieContainerHandle CookieContainerHandle) const { return SteamHTTP()->ReleaseCookieContainer(CookieContainerHandle); }

	/**
	 * Releases an HTTP request handle, freeing the memory allocated within Steam.
	 * You MUST call this when you are done using each HTTPRequestHandle that you obtained via CreateHTTPRequest!
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to release.
	 * @return bool - Returns true if the the handle was released successfully, false only if the handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool ReleaseHTTPRequest(FHTTPRequestHandle RequestHandle) const { return SteamHTTP()->ReleaseHTTPRequest(RequestHandle); }

	/**
	 * Sends an HTTP request.
	 * This call is asynchronous and provides a call result handle which you must use to track the call to its completion. If you have multiple requests in flight at the same time you can use PrioritizeHTTPRequest or DeferHTTPRequest to set the priority of the request.
	 * If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.
	 * If the data you are expecting is large, you can use SendHTTPRequestAndStreamResponse to stream the data in chunks.
	 * Triggers a HTTPRequestCompleted_t callback.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to send.
	 * @param FSteamAPICall & CallHandle - Returns a call result handle to receive the response.
	 * @return bool - Returns true upon successfully setting the parameter.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 * pCallHandle is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequest(FHTTPRequestHandle RequestHandle, FSteamAPICall& CallHandle) const { return SteamHTTP()->SendHTTPRequest(RequestHandle, (SteamAPICall_t*)&CallHandle); }

	/**
	 * Sends an HTTP request and streams the response back in chunks.
	 * This call is asynchronous and provides a call result handle which you must use to track the call to its completion. Typically you'll want to allocate a buffer associated with the request handle using the -
	 * Content-Length HTTP response field to receive the total size of the data when you receive the header via HTTPRequestHeadersReceived_t. You can then append data to that buffer as it comes in.
	 * If you have multiple requests in flight at the same time you can use PrioritizeHTTPRequest or DeferHTTPRequest to set the priority of the request.
	 * If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.
	 * If the data you are expecting is small (on the order of a few megabytes or less) then you'll likely want to use SendHTTPRequest.
	 * Triggers a HTTPRequestDataReceived_t callback.
	 * Triggers a HTTPRequestHeadersReceived_t callback.
	 * Triggers a HTTPRequestCompleted_t callback.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to send.
	 * @param FSteamAPICall & CallHandle - 	Returns a call result handle to receive the response.
	 * @return bool - Returns true upon successfully setting the parameter.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 * pCallHandle is NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequestAndStreamResponse(FHTTPRequestHandle RequestHandle, FSteamAPICall& CallHandle) const { return SteamHTTP()->SendHTTPRequestAndStreamResponse(RequestHandle, (SteamAPICall_t*)&CallHandle); }

	/**
	 * Adds a cookie to the specified cookie container that will be used with future requests.
	 *
	 * @param FHTTPCookieContainerHandle CookieContainerHandle - The cookie container to set the the cookie in.
	 * @param const FString & Host - The host to set this cookie for.
	 * @param const FString & Url - The url to set this cookie for.
	 * @param const FString & Cookie - The cookie to set.
	 * @return bool - Returns true if the cookie was set successfully. Otherwise, false if the request handle was invalid or if there was a security issue parsing the cookie.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetCookie(FHTTPCookieContainerHandle CookieContainerHandle, const FString& Host, const FString& Url, const FString& Cookie) const { return SteamHTTP()->SetCookie(CookieContainerHandle, TCHAR_TO_UTF8(*Host), TCHAR_TO_UTF8(*Url), TCHAR_TO_UTF8(*Cookie)); }

	/**
	 * Set an absolute timeout in milliseconds for the HTTP request.
	 * This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the timeout on.
	 * @param int32 Milliseconds - The length of the timeout period in milliseconds.
	 * @return bool - Returns true upon successfully setting the timeout.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestAbsoluteTimeoutMS(FHTTPRequestHandle RequestHandle, int32 Milliseconds) const { return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(RequestHandle, Milliseconds); }

	/**
	 * Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request.
	 * This is just so the caller can easily keep track of which callbacks go with which request data.
	 * Must be called before sending the request.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the context value on.
	 * @param int64 ContextValue - 	The context value to set.
	 * @return bool - Returns true upon successfully setting the context value.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestContextValue(FHTTPRequestHandle RequestHandle, int64 ContextValue) const { return SteamHTTP()->SetHTTPRequestContextValue(RequestHandle, ContextValue); }

	/**
	 * Associates a cookie container to use for an HTTP request.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to associate the cookie container with.
	 * @param FHTTPCookieContainerHandle CookieContainerHandle - The cookie container handle to associate with the request handle.
	 * @return bool - Returns true upon successfully setting the cookie container.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * hCookieContainer was invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestCookieContainer(FHTTPRequestHandle RequestHandle, FHTTPCookieContainerHandle CookieContainerHandle) const { return SteamHTTP()->SetHTTPRequestCookieContainer(RequestHandle, CookieContainerHandle); }

	/**
	 * Set a GET or POST parameter value on the HTTP request.
	 * Must be called prior to sending the request.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the parameter on.
	 * @param const FString & Name - Parameter name field.
	 * @param const FString & Value - Value to associate with the name field.
	 * @return bool - Returns true upon successfully setting the parameter.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 * pchParamName or pchParamValue are NULL.
	 * The request method set in CreateHTTPRequest is not k_EHTTPMethodGET, k_EHTTPMethodHEAD, or k_EHTTPMethodPOST.
	 * If the request method is k_EHTTPMethodPOST and a POST body has already been set with SetHTTPRequestRawPostBody.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestGetOrPostParameter(FHTTPRequestHandle RequestHandle, const FString& Name, const FString& Value) const { return SteamHTTP()->SetHTTPRequestGetOrPostParameter(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Set a request header value for the HTTP request.
	 * Must be called before sending the request.
	 * A full list of standard request fields are available here on wikipedia. The User-Agent field is explicitly disallowed as it gets overwritten when the request is sent.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the header value for.
	 * @param const FString & Name - The header name field.
	 * @param const FString & Value - Value to associate with the header name field.
	 * @return bool - Returns true upon successfully setting the header value.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 * pchHeaderName is "User-Agent".
	 * pchHeaderName or pchHeaderValue are NULL.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestHeaderValue(FHTTPRequestHandle RequestHandle, const FString& Name, const FString& Value) const { return SteamHTTP()->SetHTTPRequestHeaderValue(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Set the timeout in seconds for the HTTP request.
	 * The default timeout is 60 seconds if you don't call this. This can get bumped everytime we get more data. Use SetHTTPRequestAbsoluteTimeoutMS if you need a strict maximum timeout.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the timeout on.
	 * @param int32 TimeoutSeconds - The length of the timeout period in seconds.
	 * @return bool - Returns true upon successfully setting the timeout.
	 * Returns false under the following conditions:
	 * hRequest was invalid.
	 * The request has already been sent.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestNetworkActivityTimeout(FHTTPRequestHandle RequestHandle, int32 TimeoutSeconds) const { return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(RequestHandle, TimeoutSeconds); }

	/**
	 * Sets the body for an HTTP Post request.
	 * Will fail and return false on a GET request, and will fail if POST params have already been set for the request. Setting this raw body makes it the only contents for the post, the pchContentType parameter will -
	 * set the "content-type" header for the request to inform the server how to interpret the body.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the post body on.
	 * @param const FString & ContentType - Sets the value of the calls "content-type" http header.
	 * @param uint8 & Body - The raw POST body data to set.
	 * @return bool - Returns true upon success indicating that the content-type field and the body data have been set.
	 * Otherwise, returns false under the following conditions:
	 * hRequest was invalid.
	 * The HTTP Method set in CreateHTTPRequest is not k_EHTTPMethodPOST, k_EHTTPMethodPUT, or k_EHTTPMethodPATCH.
	 * A POST body has already been set for this request either via this function or with SetHTTPRequestGetOrPostParameter.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRawPostBody(FHTTPRequestHandle RequestHandle, const FString& ContentType, uint8& Body) const { return SteamHTTP()->SetHTTPRequestRawPostBody(RequestHandle, TCHAR_TO_UTF8(*ContentType), &Body, MAX_uint8); }

	/**
	 * Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store.
	 * This currently only works Windows and macOS.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set whether the request requires a verified certificate.
	 * @param bool bRequireVerifiedCertificate - Turn on verified certificate?
	 * @return bool - Returns true upon success. Otherwise, false if the request handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRequiresVerifiedCertificate(FHTTPRequestHandle RequestHandle, bool bRequireVerifiedCertificate) const { return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(RequestHandle, bRequireVerifiedCertificate); }

	/**
	 * Set additional user agent info for a request.
	 * This doesn't clobber the normal user agent, it just adds the extra info on the end. Sending NULL or an empty string resets the user agent info to the default value.
	 *
	 * @param FHTTPRequestHandle RequestHandle - The request handle to set the user agent info for.
	 * @param const FString & UserAgentInfo - The string to append to the end of the user agent.
	 * @return bool - Returns true upon success indicating that the user agent has been updated. Otherwise, false if the request handle is invalid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestUserAgentInfo(FHTTPRequestHandle RequestHandle, const FString& UserAgentInfo) const { return SteamHTTP()->SetHTTPRequestUserAgentInfo(RequestHandle, TCHAR_TO_UTF8(*UserAgentInfo)); }

	/** Delegates */

	/**
	 * Result when an HTTP request completes.
	 * If you're using GetHTTPStreamingResponseBodyData then you should be using the HTTPRequestHeadersReceived_t or HTTPRequestDataReceived_t.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestCompleted"))
	FOnHTTPRequestCompletedDelegate OnHTTPRequestCompletedDelegate;

	/** Triggered when a chunk of data is received from a streaming HTTP request. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestDataReceived"))
	FOnHTTPRequestDataReceivedDelegate OnHTTPRequestDataReceivedDelegate;

	/** Triggered when HTTP headers are received from a streaming HTTP request. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "OnHTTPRequestHeadersReceived"))
	FOnHTTPRequestHeadersReceivedDelegate OnHTTPRequestHeadersReceivedDelegate;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestCompleted, HTTPRequestCompleted_t, OnHTTPRequestCompletedCallback);
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestDataReceived, HTTPRequestDataReceived_t, OnHTTPRequestDataReceivedCallback);
	STEAM_CALLBACK_MANUAL(USteamHTTP, OnHTTPRequestHeadersReceived, HTTPRequestHeadersReceived_t, OnHTTPRequestHeadersReceivedCallback);
};

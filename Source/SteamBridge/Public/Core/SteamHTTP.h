// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamHTTP.generated.h"

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

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "Steam HTTP", CompactNodeTitle = "SteamHTTP"))
	static USteamHTTP* GetSteamHTTP() { return USteamHTTP::StaticClass()->GetDefaultObject<USteamHTTP>(); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	int32 CreateCookieContainer(bool bAllowResponsesToModify) { return SteamHTTP()->CreateCookieContainer(bAllowResponsesToModify); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	int32 CreateHTTPRequest(ESteamHTTPMethod HTTPRequestMethod, const FString& AbsoluteURL) { return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)HTTPRequestMethod, TCHAR_TO_UTF8(*AbsoluteURL)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool DeferHTTPRequest(int32 RequestHandle) { return SteamHTTP()->DeferHTTPRequest(RequestHandle); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPDownloadProgressPct(int32 RequestHandle, float& Percent) { return SteamHTTP()->GetHTTPDownloadProgressPct(RequestHandle, &Percent); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPRequestWasTimedOut(int32 RequestHandle, bool& bWasTimedOut) { return SteamHTTP()->GetHTTPRequestWasTimedOut(RequestHandle, &bWasTimedOut); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodyData(int32 RequestHandle, uint8& BodyDataBuffer) { return SteamHTTP()->GetHTTPResponseBodyData(RequestHandle, &BodyDataBuffer, MAX_uint32); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseBodySize(int32 RequestHandle, int32& BodySize) { uint32 Tmp = 0; BodySize = Tmp; return SteamHTTP()->GetHTTPResponseBodySize(RequestHandle, &Tmp); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderSize(int32 RequestHandle, const FString& HeaderName, int32& ResponseHeaderSize) { uint32 Tmp = 0; ResponseHeaderSize = Tmp; return SteamHTTP()->GetHTTPResponseHeaderSize(RequestHandle, TCHAR_TO_UTF8(*HeaderName), &Tmp); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPResponseHeaderValue(int32 RequestHandle, const FString& HeaderName, uint8& HeaderValueBuffer) { return SteamHTTP()->GetHTTPResponseHeaderValue(RequestHandle, TCHAR_TO_UTF8(*HeaderName), &HeaderValueBuffer, MAX_uint32); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool GetHTTPStreamingResponseBodyData(int32 RequestHandle, int32 Offset, uint8& BodyDataBuffer) { return SteamHTTP()->GetHTTPStreamingResponseBodyData(RequestHandle, Offset, &BodyDataBuffer, MAX_uint32); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool PrioritizeHTTPRequest(int32 RequestHandle) { return SteamHTTP()->PrioritizeHTTPRequest(RequestHandle); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool ReleaseCookieContainer(int32 CookieContainerHandle) { return SteamHTTP()->ReleaseCookieContainer(CookieContainerHandle); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool ReleaseHTTPRequest(int32 RequestHandle) { return SteamHTTP()->ReleaseHTTPRequest(RequestHandle); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequest(int32 RequestHandle, FSteamAPICall& CallHandle) { SteamAPICall_t Tmp = 0; CallHandle = Tmp; return SteamHTTP()->SendHTTPRequest(RequestHandle, &Tmp); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SendHTTPRequestAndStreamResponse(int32 RequestHandle, FSteamAPICall& CallHandle) { SteamAPICall_t Tmp = 0; CallHandle = Tmp; return SteamHTTP()->SendHTTPRequestAndStreamResponse(RequestHandle, &Tmp); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetCookie(int32 CookieContainerHandle, const FString& Host, const FString& Url, const FString& Cookie) { return SteamHTTP()->SetCookie(CookieContainerHandle, TCHAR_TO_UTF8(*Host), TCHAR_TO_UTF8(*Url), TCHAR_TO_UTF8(*Cookie)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestAbsoluteTimeoutMS(int32 RequestHandle, int32 Milliseconds) { return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(RequestHandle, Milliseconds); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestContextValue(int32 RequestHandle, int64 ContextValue) { return SteamHTTP()->SetHTTPRequestContextValue(RequestHandle, ContextValue); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestCookieContainer(int32 RequestHandle, int32 CookieContainerHandle) { return SteamHTTP()->SetHTTPRequestCookieContainer(RequestHandle, CookieContainerHandle); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestGetOrPostParameter(int32 RequestHandle, const FString& Name, const FString& Value) { return SteamHTTP()->SetHTTPRequestGetOrPostParameter(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestHeaderValue(int32 RequestHandle, const FString& Name, const FString& Value) { return SteamHTTP()->SetHTTPRequestHeaderValue(RequestHandle, TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Value)); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestNetworkActivityTimeout(int32 RequestHandle, int32 TimeoutSeconds) { return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(RequestHandle, TimeoutSeconds); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRawPostBody(int32 RequestHandle, const FString& ContentType, uint8& Body) { return SteamHTTP()->SetHTTPRequestRawPostBody(RequestHandle, TCHAR_TO_UTF8(*ContentType), &Body, MAX_uint8); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestRequiresVerifiedCertificate(int32 RequestHandle, bool bRequireVerifiedCertificate) { return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(RequestHandle, bRequireVerifiedCertificate); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTTP")
	bool SetHTTPRequestUserAgentInfo(int32 RequestHandle, const FString& UserAgentInfo) { return SteamHTTP()->SetHTTPRequestUserAgentInfo(RequestHandle, TCHAR_TO_UTF8(*UserAgentInfo)); }

protected:
private:
};

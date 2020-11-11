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

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTTP", meta = (DisplayName = "Steam HTTP", CompactNodeTitle = "SteamHTTP"))
	static USteamHTTP* GetSteamHTTP() { return USteamHTTP::StaticClass()->GetDefaultObject<USteamHTTP>(); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTTP")
	int32 CreateCookieContainer(bool bAllowResponsesToModify) const { return SteamHTTP()->CreateCookieContainer(bAllowResponsesToModify); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTTP")
	int32 CreateHTTPRequest(ESteamHTTPMethod HTTPRequestMethod, const FString& AbsoluteURL) const { return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)HTTPRequestMethod, TCHAR_TO_UTF8(*AbsoluteURL)); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTTP")
	bool DeferHTTPRequest(int32 RequestHandle) const { return SteamHTTP()->DeferHTTPRequest(RequestHandle); }

protected:
private:
};

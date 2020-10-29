// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamStructs.h"
#include "SteamBridgeUtils.generated.h"

/**
 *
 */
UCLASS()
class STEAMBRIDGE_API USteamBridgeUtils : public UObject
{
	GENERATED_BODY()

public:
	static void ConvertIPStringToUint32(const FString& IP, uint32& OutIP);

	static FString ConvertIPToString(uint32 IP);

	UFUNCTION(BlueprintPure, Category = "Steam|USteamBridgeUtils")
	FString GetSteamIDAsString(const FSteamID& SteamID) const;
};

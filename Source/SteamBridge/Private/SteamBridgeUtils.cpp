// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.


#include "SteamBridgeUtils.h"

void USteamBridgeUtils::ConvertIPStringToUint32(const FString& IP, uint32& OutIP)
{
	TArray<FString> ParsedIP;
	IP.ParseIntoArray(ParsedIP, TEXT("."), true);
	uint32 N0 = ParsedIP.Num() > 0 ? FCString::Atoi(*ParsedIP[0]) : 0;
	uint32 N1 = ParsedIP.Num() > 1 ? FCString::Atoi(*ParsedIP[1]) : 0;
	uint32 N2 = ParsedIP.Num() > 2 ? FCString::Atoi(*ParsedIP[2]) : 0;
	uint32 N3 = ParsedIP.Num() > 3 ? FCString::Atoi(*ParsedIP[3]) : 0;
	OutIP = (N0 << 24) + (N1 << 16) + (N2 << 8) + N3;
}

FString USteamBridgeUtils::ConvertIPToString(uint32 IP)
{
	return FString::FromInt(IP >> 24) + "." + FString::FromInt((IP >> 16) % 256) + "." + FString::FromInt((IP >> 8) % 256) + "." + FString::FromInt(IP % 256);
}

FString USteamBridgeUtils::GetSteamIDAsString(const FSteamID& SteamID)
{
	return FString::Printf(TEXT("%llu"), SteamID.Value);
}

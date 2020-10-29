// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SteamStructs.generated.h"

enum class ESteamPersonaChange : uint8;

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamID
{
	GENERATED_BODY()

	uint64 Value;

	operator uint64() const { return Value; }

	FSteamID() : Value(0) {}
	FSteamID(uint64 InSteamID) : Value(InSteamID) {}
	FSteamID(const FString& InSteamID) : Value(FCString::Strtoui64(*InSteamID, NULL, 10)) {}
};


USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamAppID
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() const { return Value; }

	FSteamAppID() : Value(0) {}
	FSteamAppID(uint32 InSteamID) : Value(InSteamID) {}
};


USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHAuthTicket
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() const { return Value; }

	FHAuthTicket() : Value(0) {}
	FHAuthTicket(uint32 InValue) : Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamAPICall
{
	GENERATED_BODY()

	uint64 Value;

	operator uint64() const { return Value; }

	FSteamAPICall() : Value(0) {}
	FSteamAPICall(uint64 InValue) : Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHSteamUser
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() const { return Value; }

	FHSteamUser() : Value(0) {}
	FHSteamUser(uint32 InValue) : Value(InValue) {}
};
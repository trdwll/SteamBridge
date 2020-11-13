// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SteamStructs.generated.h"

enum class ESteamPersonaChange : uint8;
enum class ESteamControllerSourceMode : uint8;

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

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamFriendsGroupID
{
	GENERATED_BODY()

	int16 Value;

	operator int16() const { return Value; }

	FSteamFriendsGroupID() : Value(0) {}
	FSteamFriendsGroupID(int16 InValue) : Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInputAnalogActionData
{
	GENERATED_BODY()

	ESteamControllerSourceMode Mode;
	float X;
	float Y;
	bool bActive;

	FSteamInputAnalogActionData() {}
	FSteamInputAnalogActionData(ESteamControllerSourceMode mode, float x, float y, bool bactive) : Mode(mode), X(x), Y(y), bActive(bactive) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInputDigitalActionData
{
	GENERATED_BODY()

	bool bState;
	bool bActive;

	FSteamInputDigitalActionData() {}
	FSteamInputDigitalActionData(bool bstate, bool bactive) : bState(bstate), bActive(bactive) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInputMotionData
{
	GENERATED_BODY()

	FQuat RotQuat;
	FVector PosAccel;
	FVector RotVel;

	FSteamInputMotionData() {}
	FSteamInputMotionData(const FQuat& quat, const FVector& pos, const FVector& rotvel) : RotQuat(quat), PosAccel(pos), RotVel(rotvel) {}
};
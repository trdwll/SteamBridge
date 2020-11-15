// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SteamStructs.generated.h"

enum class ESteamPersonaChange : uint8;
enum class ESteamControllerSourceMode : uint8;



USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUint64
{
	GENERATED_BODY()

	uint64 Value;

	operator uint64() const { return Value; }

	FUint64() : Value(0) {}
	FUint64(uint64 value) : Value(value) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUint32
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() const { return Value; }

	FUint32() : Value(0) {}
	FUint32(uint32 value) : Value(value) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInt32
{
	GENERATED_BODY()

	int32 Value;

	operator int32() const { return Value; }

	FInt32() : Value(0) {}
	FInt32(int32 InValue) : Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInt16
{
	GENERATED_BODY()

	int16 Value;

	operator int16() const { return Value; }

	FInt16() : Value(0) {}
	FInt16(int16 InValue) : Value(InValue) {}
};


USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamID : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamAPICall : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamItemInstanceID : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInventoryUpdateHandle : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHAuthTicket : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHSteamUser : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FAppID : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamFriendsGroupID : public FInt16 { GENERATED_BODY() using FInt16::FInt16; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInventoryResult : public FInt32 { GENERATED_BODY() using FInt32::FInt32; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamItemDef : public FInt32 { GENERATED_BODY() using FInt32::FInt32; };


USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputActionSetHandle : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputAnalogActionHandle : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputDigitalActionHandle : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputHandle : public FUint64 { GENERATED_BODY() using FUint64::FUint64; };

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHTTPCookieContainerHandle : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHTTPRequestHandle : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHHTMLBrowser : public FUint32 { GENERATED_BODY() using FUint32::FUint32; };


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

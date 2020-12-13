// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"

#include "SteamStructs.generated.h"

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUint64
{
	GENERATED_BODY()

	uint64 Value;

	operator uint64() { return Value; }
	operator uint64() const { return Value; }

	FUint64() :
		Value(0) {}
	FUint64(uint64 value) :
		Value(value) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUint32
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() { return Value; }
	operator uint32() const { return Value; }

	FUint32() :
		Value(0) {}
	FUint32(uint32 value) :
		Value(value) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInt32
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Value;

	operator int32() { return Value; }
	operator int32() const { return Value; }

	FInt32() :
		Value(0) {}
	FInt32(int32 InValue) :
		Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInt16
{
	GENERATED_BODY()

	int16 Value;

	operator int16() { return Value; }
	operator int16() const { return Value; }

	FInt16() :
		Value(0) {}
	FInt16(int16 InValue) :
		Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHServerListRequest
{
	GENERATED_BODY()

	void* Value;

	FHServerListRequest() :
		Value(nullptr) {}
	FHServerListRequest(void* InValue) :
		Value(InValue) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamID : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;

	operator CSteamID() const { return CSteamID(Value); }
	bool IsValid() const { return CSteamID(Value).IsValid(); }
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamAPICall : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamItemInstanceID : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInventoryUpdateHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHAuthTicket : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHSteamUser : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FAppID : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FAccountID : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FScreenshotHandle : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamFriendsGroupID : public FInt16
{
	GENERATED_BODY()
	using FInt16::FInt16;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInventoryResult : public FInt32
{
	GENERATED_BODY()
	using FInt32::FInt32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamItemDef : public FInt32
{
	GENERATED_BODY()
	using FInt32::FInt32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHServerQuery : public FInt32
{
	GENERATED_BODY()
	using FInt32::FInt32;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputActionSetHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputAnalogActionHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputDigitalActionHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FInputHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FPartyBeaconID : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUGCHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FPublishedFileId : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FPublishedFileUpdateHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FUGCFileWriteStreamHandle : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamLeaderboardEntries : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamLeaderboard : public FUint64
{
	GENERATED_BODY()
	using FUint64::FUint64;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHTTPCookieContainerHandle : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};
USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHTTPRequestHandle : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FHHTMLBrowser : public FUint32
{
	GENERATED_BODY()
	using FUint32::FUint32;
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
	FSteamInputAnalogActionData(ESteamControllerSourceMode mode, float x, float y, bool bactive) :
		Mode(mode), X(x), Y(y), bActive(bactive) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInputDigitalActionData
{
	GENERATED_BODY()

	bool bState;
	bool bActive;

	FSteamInputDigitalActionData() {}
	FSteamInputDigitalActionData(bool bstate, bool bactive) :
		bState(bstate), bActive(bactive) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamInputMotionData
{
	GENERATED_BODY()

	FQuat RotQuat;
	FVector PosAccel;
	FVector RotVel;

	FSteamInputMotionData() {}
	FSteamInputMotionData(const FQuat& quat, const FVector& pos, const FVector& rotvel) :
		RotQuat(quat), PosAccel(pos), RotVel(rotvel) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamItemDetails
{
	GENERATED_BODY()

	FSteamItemInstanceID ItemID;
	FSteamItemDef Definition;
	int32 Quantity;
	TArray<ESteamItemFlags_> Flags;

	FSteamItemDetails() {}
	FSteamItemDetails(FSteamItemInstanceID instance, FSteamItemDef itemdef, int32 quantity, const TArray<ESteamItemFlags_>& flags) :
		ItemID(instance), Definition(itemdef), Quantity(quantity), Flags(flags) {}
	FSteamItemDetails(const SteamItemDetails_t& details)
	{
		ItemID = details.m_itemId;
		Definition = details.m_iDefinition;
		Quantity = details.m_unQuantity;
		if (details.m_unFlags & 1 << (int32)ESteamItemFlags_::NoTrade)
		{
			Flags.Add(ESteamItemFlags_::NoTrade);
		}
		if (details.m_unFlags & 1 << (int32)ESteamItemFlags_::ItemRemoved)
		{
			Flags.Add(ESteamItemFlags_::ItemRemoved);
		}
		if (details.m_unFlags & 1 << (int32)ESteamItemFlags_::ItemConsumed)
		{
			Flags.Add(ESteamItemFlags_::ItemConsumed);
		}
	}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamPartyBeaconLocation
{
	GENERATED_BODY()

	ESteamPartyBeaconLocation Type;
	uint64 LocationID;

	operator SteamPartyBeaconLocation_t() const
	{
		SteamPartyBeaconLocation_t o;
		o.m_eType = (ESteamPartyBeaconLocationType)Type;
		o.m_ulLocationID = LocationID;
		return o;
	}

	FSteamPartyBeaconLocation() {}
	FSteamPartyBeaconLocation(ESteamPartyBeaconLocation type, uint64 id) :
		Type(Type), LocationID(id) {}
	FSteamPartyBeaconLocation(const SteamPartyBeaconLocation_t& type) :
		Type((ESteamPartyBeaconLocation)type.m_eType), LocationID(type.m_ulLocationID) {}
};

USTRUCT(BlueprintType)
struct STEAMBRIDGE_API FSteamLeaderboardEntry
{
	GENERATED_BODY()

	FSteamID SteamIDUser;
	int32 GlobalRank;
	int32 Score;
	int32 Details;
	FUGCHandle UGC;

	FSteamLeaderboardEntry() {}
	FSteamLeaderboardEntry(FSteamID steamid, int32 glob, int32 score, int32 details, FUGCHandle handle) :
		SteamIDUser(steamid), GlobalRank(glob), Score(score), Details(details), UGC(handle) {}
	FSteamLeaderboardEntry(const LeaderboardEntry_t& type) :
		SteamIDUser(type.m_steamIDUser.ConvertToUint64()), GlobalRank(type.m_nGlobalRank), Score(type.m_nScore), Details(type.m_cDetails), UGC(type.m_hUGC) {}
};

// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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

	UFUNCTION(BlueprintCallable, Category = "Steam|USteamBridgeUtils")
	static FString GetSteamIDAsString(const FSteamID& SteamID);

	// Thanks to Rama for this method
	// GetEnumValueAsString<EEnumType>("EEnumType", Value);
	template <typename TEnum>
	static FString GetEnumValueAsString(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameByValue((int64)Value).ToString();
	}

	static FString GetEnumValueAsStringParsed(const FString& Data)
	{
		FString NewString = Data;
		FString Left, Right;
		NewString.Split(TEXT("::"), &Left, &Right);
		return Right;
	}
};

// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamStructs.h"
#include "SteamEnums.h"
#include "UObject/NoExportTypes.h"

#include "SteamFriends.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSteamPersonaStateChangeDelegate, FSteamID, SteamID, ESteamPersonaChange, PersonaStateChange);

/**
 * Functions for accessing and manipulating Steam friends information.
 * https://partner.steamgames.com/doc/api/ISteamFriends
 */
UCLASS()
class STEAMBRIDGECORE_API USteamFriends final : public UObject
{
	GENERATED_BODY()

public:
	USteamFriends();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "Steam Friends", CompactNodeTitle = "SteamFriends"))
	static USteamFriends* GetFriends() { return USteamFriends::StaticClass()->GetDefaultObject<USteamFriends>(); }

	/** Get the persona of the current user. */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|USteamFriends")
	FString GetPersonaName() const;

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|USteamFriends")
	bool RequestUserInformation(const FSteamID& SteamID, bool bRequireNameOnly);

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|USteamFriends", meta = (DisplayName = "OnSteamPersonaStateChanged"))
	FOnSteamPersonaStateChangeDelegate m_SteamPersonaStateChangeCallback;

private:
	STEAM_CALLBACK_MANUAL(USteamFriends, OnSteamPersonaStateChange, PersonaStateChange_t, OnSteamPersonaStateChangeCallback);
};

// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamInventory.generated.h"

/**
 * Steam Inventory query and manipulation API.
 * https://partner.steamgames.com/doc/api/ISteamInventory
 */
UCLASS()
class STEAMBRIDGE_API USteamInventory final : public UObject
{
	GENERATED_BODY()

public:
	USteamInventory();
	~USteamInventory();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Inventory", meta = (DisplayName = "Steam Inventory", CompactNodeTitle = "SteamInventory"))
	static USteamInventory* GetSteamInventory() { return USteamInventory::StaticClass()->GetDefaultObject<USteamInventory>(); }

	/**
	 * Grant a specific one-time promotional item to the current user.
	 * This can be safely called from the client because the items it can grant can be locked down via policies in the itemdefs. One of the primary scenarios for this call is to grant an item to users who also own -
	 * a specific other game. This can be useful if your game has custom UI for showing a specific promo item to the user otherwise if you want to grant multiple promotional items then use AddPromoItems or GrantPromoItems.
	 * Any items that can be granted MUST have a "promo" attribute in their itemdef. That promo item list a set of APPIDs that the user must own to be granted this given item. This version will grant all items that -
	 * have promo attributes specified for them in the configured item definitions. This allows adding additional promotional items without having to update the game client. For example the following will allow the -
	 * item to be granted if the user owns either TF2 or SpaceWar.
	 *
	 * @param FSteamInventoryResult & ResultHandle
	 * @param FSteamItemDef ItemDef
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool AddPromoItem(FSteamInventoryResult& ResultHandle, FSteamItemDef ItemDef) const { return SteamInventory()->AddPromoItem(&ResultHandle.Value, ItemDef.Value); }

	/**
	 * Grant a specific one-time promotional items to the current user.
	 * This can be safely called from the client because the items it can grant can be locked down via policies in the itemdefs. One of the primary scenarios for this call is to grant an item to users who also own a -
	 * specific other game. If you want to grant a single promotional item then use AddPromoItem. If you want to grant all possible promo items then use GrantPromoItems.
	 * Any items that can be granted MUST have a "promo" attribute in their itemdef. That promo item list a set of APPIDs that the user must own to be granted this given item. This version will grant all items that -
	 * have promo attributes specified for them in the configured item definitions. This allows adding additional promotional items without having to update the game client. For example the following will allow the item -
	 * to be granted if the user owns either TF2 or SpaceWar.
	 *
	 * @param FSteamInventoryResult & ResultHandle
	 * @param const TArray<FSteamItemDef> & ItemDefs
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool AddPromoItems(FSteamInventoryResult& ResultHandle, const TArray<FSteamItemDef>& ItemDefs) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool CheckResultSteamID(FSteamInventoryResult ResultHandle, FSteamID SteamIDExpected) const { return SteamInventory()->CheckResultSteamID(ResultHandle, SteamIDExpected.Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool ConsumeItem(FSteamInventoryResult& ResultHandle, FSteamItemInstanceID ItemID, int32 Quantity) const { return SteamInventory()->ConsumeItem(&ResultHandle.Value, ItemID, Quantity); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool DeserializeResult(FSteamInventoryResult& ResultHandle, TArray<uint8> Buffer) const { return SteamInventory()->DeserializeResult(&ResultHandle.Value, Buffer.GetData(), Buffer.Num(), false); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Inventory")
	void DestroyResult(FSteamInventoryResult ResultHandle) { SteamInventory()->DestroyResult(ResultHandle); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool ExchangeItems(FSteamInventoryResult& ResultHandle, const TMap<FSteamItemDef, int32>& ItemsGenerated, const TMap<FSteamItemInstanceID, int32>& ItemsDestroyed) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool GenerateItems(FSteamInventoryResult& ResultHandle, const TMap<FSteamItemDef, int32>& Items) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool GetAllItems(FSteamInventoryResult& ResultHandle) const { return SteamInventory()->GetAllItems(&ResultHandle.Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool GetEligiblePromoItemDefinitionIDs(FSteamID SteamID, TArray<FSteamItemDef>& Items) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Inventory")
	bool GetItemDefinitionIDs(TArray<FSteamItemDef>& Items) const;

	// TODO: GetItemDefinitionProperty
	// TODO: GetItemsByID

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Inventory")
	bool GetItemPrice(FSteamItemDef ItemDef, int64& CurrentPrice, int64& BasePrice) const;

	// TODO: GetItemsWithPrices

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Inventory")
	int32 GetNumItemsWithPrices() const { return SteamInventory()->GetNumItemsWithPrices(); }

protected:
private:
};

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

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool AddPromoItem(FSteamInventoryResult& ResultHandle, FSteamItemDef ItemDef) const { return SteamInventory()->AddPromoItem(&ResultHandle.Value, ItemDef.Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool AddPromoItems(FSteamInventoryResult& ResultHandle, const TArray<FSteamItemDef>& ItemDefs) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool CheckResultSteamID(FSteamInventoryResult ResultHandle, FSteamID SteamIDExpected) const { return SteamInventory()->CheckResultSteamID(ResultHandle, SteamIDExpected.Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool ConsumeItem(FSteamInventoryResult& ResultHandle, FSteamItemInstanceID ItemID, int32 Quantity) const { return SteamInventory()->ConsumeItem(&ResultHandle.Value, ItemID, Quantity); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool DeserializeResult(FSteamInventoryResult& ResultHandle, TArray<uint8> Buffer) const { return SteamInventory()->DeserializeResult(&ResultHandle.Value, Buffer.GetData(), Buffer.Num(), false); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|USteamInventory")
	void DestroyResult(FSteamInventoryResult ResultHandle) { SteamInventory()->DestroyResult(ResultHandle); }

	// TODO: ExchangeItems
	// TODO: GenerateItems

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool GetAllItems(FSteamInventoryResult& ResultHandle) const { return SteamInventory()->GetAllItems(&ResultHandle.Value); }

	// TODO: GetEligiblePromoItemDefinitionIDs
	// TODO: GetItemDefinitionIDs
	// TODO: GetItemDefinitionProperty
	// TODO: GetItemsByID

	bool GetItemPrice(FSteamItemDef ItemDef, int64& CurrentPrice, int64& BasePrice) const;

	// TODO: GetItemsWithPrices

	int32 GetNumItemsWithPrices() const { return SteamInventory()->GetNumItemsWithPrices(); }

protected:
private:
};

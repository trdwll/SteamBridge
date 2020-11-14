// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamInventory.generated.h"

// NOTE: ResultHandle = SteamInventoryResult_t (int32)
// NOTE: ItemDef = SteamItemDef_t (int32)
// NOTE: ItemID = SteamItemInstanceID_t (uint64)
// NOTE: UpdateHandle = SteamInventoryUpdateHandle_t (uint64)

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
	bool AddPromoItem(int32& ResultHandle, int32 ItemDef) const { return SteamInventory()->AddPromoItem(&ResultHandle, ItemDef); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool AddPromoItems(int32& ResultHandle, const TArray<int32>& ItemDefs) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool CheckResultSteamID(int32 ResultHandle, FSteamID SteamIDExpected) const { return SteamInventory()->CheckResultSteamID(ResultHandle, SteamIDExpected.Value); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool ConsumeItem(int32& ResultHandle, int64 ItemID, int32 Quantity) const { return SteamInventory()->ConsumeItem(&ResultHandle, ItemID, Quantity); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool DeserializeResult(int32& ResultHandle, TArray<uint8> Buffer) const { return SteamInventory()->DeserializeResult(&ResultHandle, Buffer.GetData(), Buffer.Num(), false); }

	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|USteamInventory")
	void DestroyResult(int32 ResultHandle) { SteamInventory()->DestroyResult(ResultHandle); }

	// TODO: ExchangeItems
	// TODO: GenerateItems

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|USteamInventory")
	bool GetAllItems(int32& ResultHandle) const { return SteamInventory()->GetAllItems(&ResultHandle); }

	// TODO: GetEligiblePromoItemDefinitionIDs
	// TODO: GetItemDefinitionIDs
	// TODO: GetItemDefinitionProperty
	// TODO: GetItemsByID

	bool GetItemPrice(int32 ItemDef, int64& CurrentPrice, int64& BasePrice) const;

	// TODO: GetItemsWithPrices

	int32 GetNumItemsWithPrices() const { return SteamInventory()->GetNumItemsWithPrices(); }




protected:
private:
};

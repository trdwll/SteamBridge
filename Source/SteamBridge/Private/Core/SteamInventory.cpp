// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamInventory.h"

#include "SteamBridgeUtils.h"

USteamInventory::USteamInventory()
{
}

USteamInventory::~USteamInventory()
{
}

bool USteamInventory::AddPromoItems(FSteamInventoryResult& ResultHandle, const TArray<FSteamItemDef>& ItemDefs) const
{
	TArray<int32> Tmp;
	for (int32 i = 0; i < ItemDefs.Num(); i++)
	{
		Tmp.Add(ItemDefs[i]);
	}

	return SteamInventory()->AddPromoItems(&ResultHandle.Value, Tmp.GetData(), Tmp.Num());
}

bool USteamInventory::ExchangeItems(FSteamInventoryResult& ResultHandle, const TMap<FSteamItemDef, int32>& ItemsGenerated, const TMap<FSteamItemInstanceID, int32>& ItemsDestroyed) const
{
	// NOTE: I converted this method to support a TMap instead of 4 arrays so I'm not entirely sure if it works lemme know - trdwll
	if (ItemsGenerated.Num() < 1 || ItemsDestroyed.Num() < 1) return false;

	TArray<SteamItemDef_t> TmpItemsGenerated;
	TArray<SteamItemInstanceID_t> TmpItemsDestroyed;
	TArray<uint32> TmpItemsGeneratedQuantity;
	TmpItemsGeneratedQuantity.Init(1, TmpItemsGenerated.Num());
	TArray<uint32> TmpItemsDestroyedQuantity;
	TmpItemsDestroyedQuantity.Init(1, TmpItemsDestroyed.Num());

	for (const auto& pair : ItemsGenerated)
	{
		TmpItemsGenerated.Add(pair.Key);
	}
	for (const auto& pair : ItemsDestroyed)
	{
		TmpItemsDestroyed.Add(pair.Key);
	}
	return SteamInventory()->ExchangeItems(&ResultHandle.Value, TmpItemsGenerated.GetData(), TmpItemsGeneratedQuantity.GetData(), ItemsGenerated.Num(), TmpItemsDestroyed.GetData(), TmpItemsDestroyedQuantity.GetData(), ItemsDestroyed.Num());
}

bool USteamInventory::GenerateItems(FSteamInventoryResult& ResultHandle, const TMap<FSteamItemDef, int32>& Items) const
{
	// NOTE: I converted this method to support a TMap instead of 2 arrays so I'm not entirely sure if it works lemme know - trdwll

	TArray<SteamItemDef_t> TmpItems;

	for (const auto& pair : Items)
	{
		TmpItems.Add(pair.Key);
	}

	return SteamInventory()->GenerateItems(&ResultHandle.Value, TmpItems.GetData(), (uint32*)TmpItems.GetData(), TmpItems.Num());
}

bool USteamInventory::GetEligiblePromoItemDefinitionIDs(FSteamID SteamID, TArray<FSteamItemDef>& Items) const
{
	uint32 TmpCount = 0;
	if (SteamInventory()->GetEligiblePromoItemDefinitionIDs(SteamID.Value, nullptr, &TmpCount))
	{
		TArray<SteamItemDef_t> TmpArray;
		TmpArray.SetNum(TmpCount);
		bool result = SteamInventory()->GetEligiblePromoItemDefinitionIDs(SteamID.Value, TmpArray.GetData(), &TmpCount);

		for (int32 i = 0; i < (int32)TmpCount; i++)
		{
			Items.Add(TmpArray[i]);
		}

		return result;
	}
	return false;
}

bool USteamInventory::GetItemDefinitionIDs(TArray<FSteamItemDef>& Items) const
{
	uint32 TmpCount = 0;
	if (SteamInventory()->GetItemDefinitionIDs(nullptr, &TmpCount))
	{
		TArray<SteamItemDef_t> TmpArray;
		TmpArray.SetNum(TmpCount);
		bool result = SteamInventory()->GetItemDefinitionIDs(TmpArray.GetData(), &TmpCount);

		for (int32 i = 0; i < (int32)TmpCount; i++)
		{
			Items.Add(TmpArray[i]);
		}

		return result;
	}
	return false;
}

bool USteamInventory::GetItemPrice(FSteamItemDef ItemDef, int64& CurrentPrice, int64& BasePrice) const
{
	uint64 TmpCurrent = 0, TmpBase = 0;
	CurrentPrice = TmpCurrent;
	BasePrice = TmpBase;
	return SteamInventory()->GetItemPrice(ItemDef.Value, &TmpCurrent, &TmpBase);
}

bool USteamInventory::GetResultItemProperty(FSteamInventoryResult ResultHandle, int32 ItemIndex, const FString& PropertyName, FString& Value) const
{
	TArray<char> TmpStr;
	uint32 TmpStringValue = 0;
	bool result = SteamInventory()->GetResultItemProperty(ResultHandle, ItemIndex, TCHAR_TO_UTF8(*PropertyName), TmpStr.GetData(), (uint32*)TmpStringValue);
	Value = UTF8_TO_TCHAR(TmpStr.GetData());
	return result;
}

bool USteamInventory::GetResultItems(FSteamInventoryResult ResultHandle, TArray<FSteamItemDetails>& ItemsArray) const
{
	uint32 TmpCount = 0;

	if (SteamInventory()->GetResultItems(ResultHandle, nullptr, &TmpCount))
	{
		TArray<SteamItemDetails_t> TmpArray;
		TmpArray.SetNum(TmpCount);
		bool result = SteamInventory()->GetResultItems(ResultHandle, TmpArray.GetData(), &TmpCount);

		for (int32 i = 0; i < (int32)TmpCount; i++)
		{
			ItemsArray.Add((FSteamItemDetails)TmpArray[i]);
		}

		return result;
	}

	return false;
}

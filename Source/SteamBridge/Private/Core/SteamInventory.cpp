// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamInventory.h"

#include "SteamBridgeUtils.h"

USteamInventory::USteamInventory()
{
	OnSteamInventoryDefinitionUpdateCallback.Register(this, &USteamInventory::OnSteamInventoryDefinitionUpdate);
	OnSteamInventoryEligiblePromoItemDefIDsCallback.Register(this, &USteamInventory::OnSteamInventoryEligiblePromoItemDefIDs);
	OnSteamInventoryFullUpdateCallback.Register(this, &USteamInventory::OnSteamInventoryFullUpdate);
	OnSteamInventoryResultReadyCallback.Register(this, &USteamInventory::OnSteamInventoryResultReady);
	OnSteamInventoryStartPurchaseResultCallback.Register(this, &USteamInventory::OnSteamInventoryStartPurchaseResult);
	OnSteamInventoryRequestPricesResultCallback.Register(this, &USteamInventory::OnSteamInventoryRequestPricesResult);
}

USteamInventory::~USteamInventory()
{
	OnSteamInventoryDefinitionUpdateCallback.Unregister();
	OnSteamInventoryEligiblePromoItemDefIDsCallback.Unregister();
	OnSteamInventoryFullUpdateCallback.Unregister();
	OnSteamInventoryResultReadyCallback.Unregister();
	OnSteamInventoryStartPurchaseResultCallback.Unregister();
	OnSteamInventoryRequestPricesResultCallback.Unregister();
}

bool USteamInventory::AddPromoItems(FSteamInventoryResult& ResultHandle, const TArray<FSteamItemDef>& ItemDefs) const
{
	TArray<int32> Tmp;
	for (const auto& Item : ItemDefs)
	{
		Tmp.Add(Item);
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
	if (SteamInventory()->GetEligiblePromoItemDefinitionIDs(SteamID, nullptr, &TmpCount))
	{
		TArray<SteamItemDef_t> TmpArray;
		TmpArray.SetNum(TmpCount);
		bool result = SteamInventory()->GetEligiblePromoItemDefinitionIDs(SteamID, TmpArray.GetData(), &TmpCount);

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

bool USteamInventory::GetItemDefinitionProperty(FSteamItemDef Definition, const FString& PropertyName, FString& Value) const
{
	uint32 Size = 0;
	if (SteamInventory()->GetItemDefinitionProperty(Definition, TCHAR_TO_UTF8(*PropertyName), nullptr, &Size))
	{
		TArray<char> TmpStr;
		TmpStr.SetNum(Size);
		bool bResult = SteamInventory()->GetItemDefinitionProperty(Definition, TCHAR_TO_UTF8(*PropertyName), TmpStr.GetData(), &Size);
		Value = UTF8_TO_TCHAR(TmpStr.GetData());
		return bResult;
	}
	return false;
}

bool USteamInventory::GetItemsByID(FSteamInventoryResult& ResultHandle, const TArray<FSteamItemInstanceID>& InstanceIDs) const
{
	const int32 Size = InstanceIDs.Num();
	TArray<SteamItemInstanceID_t> TmpIDs;
	TmpIDs.SetNum(Size);

	for (const auto& ID : InstanceIDs)
	{
		TmpIDs.Add(ID);
	}

	return SteamInventory()->GetItemsByID((SteamInventoryResult_t*)&ResultHandle, TmpIDs.GetData(), Size);
}

bool USteamInventory::GetItemsWithPrices(TArray<FSteamItemPriceData>& ItemData) const
{
	const int32 Size = SteamInventory()->GetNumItemsWithPrices();

	TArray<SteamItemDef_t> TmpItems;
	TArray<uint64> TmpCurrentPrices;
	TArray<uint64> TmpBasePrices;
	TmpItems.SetNum(Size);
	TmpCurrentPrices.SetNum(Size);
	TmpBasePrices.SetNum(Size);

	if (bool bResult = SteamInventory()->GetItemsWithPrices(TmpItems.GetData(), TmpCurrentPrices.GetData(), TmpBasePrices.GetData(), Size))
	{
		for (int32 i = 0; i < Size; i++)
		{
			ItemData.Add({TmpItems[i], (int64)TmpCurrentPrices[i], (int64)TmpBasePrices[i] });
		}
		return bResult;
	}
	return false;
}

bool USteamInventory::GetResultItemProperty(FSteamInventoryResult ResultHandle, int32 ItemIndex, const FString& PropertyName, FString& Value) const
{
	TArray<char> TmpStr;
	uint32 TmpStringValue = 0;
	bool result = SteamInventory()->GetResultItemProperty(ResultHandle, ItemIndex, TCHAR_TO_UTF8(*PropertyName), TmpStr.GetData(), &TmpStringValue);
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

bool USteamInventory::SerializeResult(FSteamInventoryResult ResultHandle, TArray<uint8>& Buffer) const
{
	uint32 TmpCount = 0;
	if (SteamInventory()->SerializeResult(ResultHandle, nullptr, &TmpCount))
	{
		Buffer.SetNum(TmpCount);
		bool bResult = SteamInventory()->SerializeResult(ResultHandle, Buffer.GetData(), &TmpCount);
		return bResult;
	}

	return false;
}

bool USteamInventory::TransferItemQuantity(FSteamInventoryResult& ResultHandle, FSteamItemInstanceID ItemIdSource, int32 Quantity, FSteamItemInstanceID ItemIdDest) const
{
	return SteamInventory()->TransferItemQuantity(&ResultHandle.Value, ItemIdSource, Quantity, ItemIdDest);
}

bool USteamInventory::SetPropertyString(FSteamInventoryUpdateHandle UpdateHandle, FSteamItemInstanceID ItemID, const FString& PropertyName, const FString& PropertyValue) const
{
	return SteamInventory()->SetProperty(UpdateHandle, ItemID, TCHAR_TO_UTF8(*PropertyName), TCHAR_TO_UTF8(*PropertyValue));
}

void USteamInventory::OnSteamInventoryDefinitionUpdate(SteamInventoryDefinitionUpdate_t* pParam)
{
	OnSteamInventoryDefinitionUpdateDelegate.Broadcast();
}

void USteamInventory::OnSteamInventoryEligiblePromoItemDefIDs(SteamInventoryEligiblePromoItemDefIDs_t* pParam)
{
	OnSteamInventoryEligiblePromoItemDefIDsDelegate.Broadcast((ESteamResult)pParam->m_result, pParam->m_steamID.ConvertToUint64(), pParam->m_numEligiblePromoItemDefs, pParam->m_bCachedData);
}

void USteamInventory::OnSteamInventoryFullUpdate(SteamInventoryFullUpdate_t* pParam)
{
	OnSteamInventoryFullUpdateDelegate.Broadcast(pParam->m_handle);
}

void USteamInventory::OnSteamInventoryResultReady(SteamInventoryResultReady_t* pParam)
{
	OnSteamInventoryResultReadyDelegate.Broadcast(pParam->m_handle, (ESteamResult)pParam->m_result);
}

void USteamInventory::OnSteamInventoryStartPurchaseResult(SteamInventoryStartPurchaseResult_t* pParam)
{
	OnSteamInventoryStartPurchaseResultDelegate.Broadcast((ESteamResult)pParam->m_result, pParam->m_ulOrderID, pParam->m_ulTransID);
}

void USteamInventory::OnSteamInventoryRequestPricesResult(SteamInventoryRequestPricesResult_t* pParam)
{
	OnSteamInventoryRequestPricesResultDelegate.Broadcast((ESteamResult)pParam->m_result, UTF8_TO_TCHAR(pParam->m_rgchCurrency));
}

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

bool USteamInventory::GetItemPrice(FSteamItemDef ItemDef, int64& CurrentPrice, int64& BasePrice) const
{
	uint64 TmpCurrent = 0, TmpBase = 0;
	CurrentPrice = TmpCurrent;
	BasePrice = TmpBase;
	return SteamInventory()->GetItemPrice(ItemDef.Value, &TmpCurrent, &TmpBase);
}

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

bool USteamInventory::AddPromoItems(int32& ResultHandle, const TArray<int32>& ItemDefs) const
{
	TArray<int32> Tmp;
	for (int32 i = 0; i < ItemDefs.Num(); i++)
	{
		Tmp.Add(ItemDefs[i]);
	}

	return SteamInventory()->AddPromoItems(&ResultHandle, Tmp.GetData(), Tmp.Num());
}

bool USteamInventory::GetItemPrice(int32 ItemDef, int64& CurrentPrice, int64& BasePrice) const
{
	uint64 TmpCurrent = 0, TmpBase = 0;
	CurrentPrice = TmpCurrent;
	BasePrice = TmpBase;
	return SteamInventory()->GetItemPrice(ItemDef, &TmpCurrent, &TmpBase);
}

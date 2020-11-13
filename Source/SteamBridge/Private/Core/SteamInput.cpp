// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamInput.h"

#include "SteamBridgeUtils.h"

USteamInput::USteamInput()
{
}

USteamInput::~USteamInput()
{
}

int32 USteamInput::GetActiveActionSetLayers(int64 InputHandle, TArray<int64>& Handles)
{
	TArray<uint64> Tmp;
	int32 result = SteamInput()->GetActiveActionSetLayers(InputHandle, Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Handles.Add(Tmp[i]);
	}

	return result;
}

FSteamInputAnalogActionData USteamInput::GetAnalogActionData(int64 InputHandle, int64 ActionSetLayerHandle) const
{
	InputAnalogActionData_t data = SteamInput()->GetAnalogActionData(InputHandle, ActionSetLayerHandle);
	return {(ESteamControllerSourceMode)data.eMode, data.x, data.y, data.bActive};
}

int32 USteamInput::GetAnalogActionOrigins(int64 InputHandle, int64 ActionSetHandle, int64 AnalogActionHandle, TArray<ESteamInputActionOrigin>& Origins)
{
	TArray<EInputActionOrigin> Tmp;
	int32 result = SteamInput()->GetAnalogActionOrigins(InputHandle, ActionSetHandle, AnalogActionHandle, Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Origins.Add((ESteamInputActionOrigin)Tmp[i]);
	}

	return result;
}

int32 USteamInput::GetConnectedControllers(TArray<int64>& Handles)
{
	TArray<uint64> Tmp;
	int32 result = SteamInput()->GetConnectedControllers(Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Handles.Add(Tmp[i]);
	}

	return result;
}

FSteamInputDigitalActionData USteamInput::GetDigitalActionData(int64 InputHandle, int64 DigitalActionHandle) const
{
	InputDigitalActionData_t data = SteamInput()->GetDigitalActionData(InputHandle, DigitalActionHandle);
	return {data.bState, data.bActive};
}

int32 USteamInput::GetDigitalActionOrigins(int64 InputHandle, int64 ActionSetHandle, int64 DigitalActionHandle, TArray<ESteamInputActionOrigin>& Origins)
{
	TArray<EInputActionOrigin> Tmp;
	int32 result = SteamInput()->GetDigitalActionOrigins(InputHandle, ActionSetHandle, DigitalActionHandle, Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Origins.Add((ESteamInputActionOrigin)Tmp[i]);
	}

	return result;
}

FSteamInputMotionData USteamInput::GetMotionData(int64 InputHandle) const
{
	InputMotionData_t data = SteamInput()->GetMotionData(InputHandle);
	return {{data.rotQuatX, data.rotQuatY, data.rotQuatZ, data.rotQuatW}, {data.posAccelX, data.posAccelY, data.posAccelZ}, {data.rotVelX, data.rotVelY, data.rotVelZ}};
}

void USteamInput::SetLEDColorU(int64 InputHandle, uint8 R, uint8 G, uint8 B, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, R, G, B, Tmp);
}

void USteamInput::SetLEDColor(int64 InputHandle, const FLinearColor& Color, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, Color.R, Color.G, Color.B, Tmp);
}

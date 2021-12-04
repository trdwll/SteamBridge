// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamInput.h"

#include "SteamBridgeUtils.h"

USteamInput::USteamInput()
{
}

USteamInput::~USteamInput()
{
}

int32 USteamInput::GetActiveActionSetLayers(FInputHandle InputHandle, TArray<FInputHandle>& Handles)
{
	TArray<uint64> Tmp;
	int32 result = SteamInput()->GetActiveActionSetLayers(InputHandle, Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Handles.Add(Tmp[i]);
	}

	return result;
}

FSteamInputAnalogActionData USteamInput::GetAnalogActionData(FInputHandle InputHandle, FInputActionSetHandle ActionSetLayerHandle) const
{
	InputAnalogActionData_t data = SteamInput()->GetAnalogActionData(InputHandle, ActionSetLayerHandle);
	return {(ESteamControllerSourceMode)data.eMode, data.x, data.y, data.bActive};
}

int32 USteamInput::GetAnalogActionOrigins(FInputHandle InputHandle, FInputActionSetHandle ActionSetHandle, FInputAnalogActionHandle AnalogActionHandle, TArray<ESteamInputActionOrigin>& Origins)
{
	TArray<EInputActionOrigin> AnalogActionOrigins;

	AnalogActionOrigins.Init(k_EInputActionOrigin_None, STEAM_INPUT_MAX_ORIGINS);

	int32 result = SteamInput()->GetAnalogActionOrigins(InputHandle, ActionSetHandle, AnalogActionHandle, AnalogActionOrigins.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Origins.Add((ESteamInputActionOrigin)AnalogActionOrigins[i]);
	}

	return result;
}

int32 USteamInput::GetConnectedControllers(TArray<FInputHandle>& Handles)
{
	TArray<InputHandle_t> ConnectedControllers;

	ConnectedControllers.Init(0, STEAM_INPUT_MAX_COUNT);

	int32 result = SteamInput()->GetConnectedControllers(ConnectedControllers.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Handles.Add(ConnectedControllers[i]);
	}

	return result;
}

FSteamInputDigitalActionData USteamInput::GetDigitalActionData(FInputHandle InputHandle, FInputDigitalActionHandle DigitalActionHandle) const
{
	InputDigitalActionData_t data = SteamInput()->GetDigitalActionData(InputHandle, DigitalActionHandle);
	return {data.bState, data.bActive};
}

int32 USteamInput::GetDigitalActionOrigins(FInputHandle InputHandle, FInputActionSetHandle ActionSetHandle, FInputDigitalActionHandle DigitalActionHandle, TArray<ESteamInputActionOrigin>& Origins)
{
	TArray<EInputActionOrigin> ActionOrigins;

	ActionOrigins.Init(k_EInputActionOrigin_None, STEAM_INPUT_MAX_ORIGINS);

	int32 result = SteamInput()->GetDigitalActionOrigins(InputHandle, ActionSetHandle, DigitalActionHandle, ActionOrigins.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Origins.Add((ESteamInputActionOrigin)ActionOrigins[i]);
	}

	return result;
}

FSteamInputMotionData USteamInput::GetMotionData(FInputHandle InputHandle) const
{
	InputMotionData_t data = SteamInput()->GetMotionData(InputHandle);
	return {{data.rotQuatX, data.rotQuatY, data.rotQuatZ, data.rotQuatW}, {data.posAccelX, data.posAccelY, data.posAccelZ}, {data.rotVelX, data.rotVelY, data.rotVelZ}};
}

void USteamInput::SetLEDColorU(FInputHandle InputHandle, uint8 R, uint8 G, uint8 B, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, R, G, B, Tmp);
}

void USteamInput::SetLEDColor(FInputHandle InputHandle, const FLinearColor& Color, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, Color.R, Color.G, Color.B, Tmp);
}

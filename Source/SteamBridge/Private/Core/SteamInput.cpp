// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamInput.h"

#include "SteamBridgeUtils.h"

int32 USteamInput::GetActiveActionSetLayers(FInputHandle InputHandle, TArray<FInputHandle>& Handles)
{
	TArray<uint64> Tmp;
	Tmp.Reserve(SteamDefs::Buffer128);
	int32 result = SteamInput()->GetActiveActionSetLayers(InputHandle, Tmp.GetData());

	for (int32 i = 0; i < result; i++)
	{
		Handles.Add(Tmp[i]);
	}

	return result;
}

FSteamInputAnalogActionData USteamInput::GetAnalogActionData(const FInputHandle InputHandle, const FInputAnalogActionHandle AnalogActionHandle) const
{
	InputAnalogActionData_t data = SteamInput()->GetAnalogActionData(InputHandle, AnalogActionHandle);
	return {(ESteamControllerSourceMode)data.eMode, data.x, data.y, data.bActive};
}

int32 USteamInput::GetAnalogActionOrigins(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle, const FInputAnalogActionHandle AnalogActionHandle, TArray<ESteamInputActionOrigin>& Origins)
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

FSteamInputDigitalActionData USteamInput::GetDigitalActionData(const FInputHandle InputHandle, const FInputDigitalActionHandle DigitalActionHandle) const
{
	InputDigitalActionData_t data = SteamInput()->GetDigitalActionData(InputHandle, DigitalActionHandle);
	return {data.bState, data.bActive};
}

int32 USteamInput::GetDigitalActionOrigins(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle, const FInputDigitalActionHandle DigitalActionHandle, TArray<ESteamInputActionOrigin>& Origins)
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

FSteamInputMotionData USteamInput::GetMotionData(const FInputHandle InputHandle) const
{
	InputMotionData_t data = SteamInput()->GetMotionData(InputHandle);
	return {{data.rotQuatX, data.rotQuatY, data.rotQuatZ, data.rotQuatW}, {data.posAccelX, data.posAccelY, data.posAccelZ}, {data.rotVelX, data.rotVelY, data.rotVelZ}};
}

void USteamInput::SetLEDColorU(const FInputHandle InputHandle, const uint8 R, const uint8 G, const uint8 B, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, R, G, B, Tmp);
}

void USteamInput::SetLEDColor(const FInputHandle InputHandle, const FLinearColor& Color, const TArray<ESteamControllerLEDFlag_>& Flags)
{
	int Tmp = 0;
	for (int i = 0; i < Flags.Num(); i++)
	{
		Tmp |= 1 << (uint8)Flags[i];
	}
	SteamInput()->SetLEDColor(InputHandle, Color.R, Color.G, Color.B, Tmp);
}

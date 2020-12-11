// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUtils.h"

#include "SteamBridgeUtils.h"

USteamUtils::USteamUtils()
{
	OnGamepadTextInputDismissedCallback.Register(this, &USteamUtils::OnGamepadTextInputDismissed);
	OnIPCountryCallback.Register(this, &USteamUtils::OnIPCountry);
	OnLowBatteryPowerCallback.Register(this, &USteamUtils::OnLowBatteryPower);
	OnSteamAPICallCompletedCallback.Register(this, &USteamUtils::OnSteamAPICallCompleted);
	OnSteamShutdownCallback.Register(this, &USteamUtils::OnSteamShutdown);
}

USteamUtils::~USteamUtils()
{
	OnGamepadTextInputDismissedCallback.Unregister();
	OnIPCountryCallback.Unregister();
	OnLowBatteryPowerCallback.Unregister();
	OnSteamAPICallCompletedCallback.Unregister();
	OnSteamShutdownCallback.Unregister();
}

bool USteamUtils::GetCSERIPPort(FString& IP, int32& Port) const
{
	uint32 TmpIP;
	uint16 TmpPort;
	bool bResult = SteamUtils()->GetCSERIPPort(&TmpIP, &TmpPort);
	IP = USteamBridgeUtils::ConvertIPToString(TmpIP);
	Port = TmpPort;
	return bResult;
}

bool USteamUtils::GetEnteredGamepadTextInput(FString& Text) const
{
	TArray<char> TmpStr;
	bool bResult = SteamUtils()->GetEnteredGamepadTextInput(TmpStr.GetData(), 8192);
	Text = UTF8_TO_TCHAR(TmpStr.GetData());
	return bResult;
}

bool USteamUtils::ShowGamepadTextInput(ESteamGamepadTextInputMode InputMode, ESteamGamepadTextInputLineMode LineInputMode, const FString& Description, const FString& ExistingText, int32 CharMax) const
{
	return SteamUtils()->ShowGamepadTextInput((EGamepadTextInputMode)InputMode, (EGamepadTextInputLineMode)LineInputMode, TCHAR_TO_UTF8(*Description), CharMax, TCHAR_TO_UTF8(*ExistingText));
}

void USteamUtils::OnGamepadTextInputDismissed(GamepadTextInputDismissed_t* pParam)
{
	m_OnGamepadTextInputDismissed.Broadcast(pParam->m_bSubmitted, pParam->m_unSubmittedText);
}

void USteamUtils::OnIPCountry(IPCountry_t* pParam)
{
	m_OnIPCountry.Broadcast();
}

void USteamUtils::OnLowBatteryPower(LowBatteryPower_t* pParam)
{
	m_OnLowBatteryPower.Broadcast(pParam->m_nMinutesBatteryLeft);
}

void USteamUtils::OnSteamAPICallCompleted(SteamAPICallCompleted_t* pParam)
{
	m_OnSteamAPICallCompleted.Broadcast((FSteamAPICall)pParam->m_hAsyncCall, pParam->m_iCallback, pParam->m_cubParam);
}

void USteamUtils::OnSteamShutdown(SteamShutdown_t* pParam)
{
	m_OnSteamShutdown.Broadcast();
}

// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "SteamFriends.h"

#include "Steam.h"

USteamFriends::USteamFriends()
{
	OnSteamPersonaStateChangeCallback.Register(this, &USteamFriends::OnSteamPersonaStateChange);
}

FString USteamFriends::GetPersonaName() const
{
	return SteamFriends()->GetPersonaName();
}

bool USteamFriends::RequestUserInformation(const FSteamID& SteamID, bool bRequireNameOnly)
{
	return SteamFriends()->RequestUserInformation(SteamID.Value, bRequireNameOnly);
}

void USteamFriends::OnSteamPersonaStateChange(PersonaStateChange_t* pParam)
{
	m_SteamPersonaStateChangeCallback.Broadcast(pParam->m_ulSteamID, static_cast<ESteamPersonaChange>((uint8)pParam->m_nChangeFlags));
}

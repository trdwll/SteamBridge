// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamParties.h"

#include "SteamBridgeUtils.h"

USteamParties::USteamParties()
{
	OnJoinPartyCallback.Register(this, &USteamParties::OnJoinParty);
	OnCreateBeaconCallback.Register(this, &USteamParties::OnCreateBeacon);
	OnReservationNotificationCallback.Register(this, &USteamParties::OnReservationNotification);
	OnChangeNumOpenSlotsCallback.Register(this, &USteamParties::OnChangeNumOpenSlots);
	OnAvailableBeaconLocationsUpdatedCallback.Register(this, &USteamParties::OnAvailableBeaconLocationsUpdated);
	OnActiveBeaconsUpdatedCallback.Register(this, &USteamParties::OnActiveBeaconsUpdated);
}

USteamParties::~USteamParties()
{
	OnJoinPartyCallback.Unregister();
	OnCreateBeaconCallback.Unregister();
	OnReservationNotificationCallback.Unregister();
	OnChangeNumOpenSlotsCallback.Unregister();
	OnAvailableBeaconLocationsUpdatedCallback.Unregister();
	OnActiveBeaconsUpdatedCallback.Unregister();
}

bool USteamParties::GetAvailableBeaconLocations(TArray<FSteamPartyBeaconLocation>& LocationList) const
{
	TArray<SteamPartyBeaconLocation_t> TmpArray;
	int32 Num = 0;
	GetNumAvailableBeaconLocations(Num);
	bool bResult = SteamParties()->GetAvailableBeaconLocations(TmpArray.GetData(), Num);

	for (int32 i = 0; i < TmpArray.Num(); i++)
	{
		LocationList.Add(TmpArray[i]);
	}

	return bResult;
}

FSteamAPICall USteamParties::CreateBeacon(int32 OpenSlots, FSteamPartyBeaconLocation& BeaconLocation, const FString& ConnectString, const FString& Metadata) const
{
	SteamPartyBeaconLocation_t TmpLocation;
	FSteamAPICall result = SteamParties()->CreateBeacon(OpenSlots, &TmpLocation, TCHAR_TO_UTF8(*ConnectString), TCHAR_TO_UTF8(*Metadata));
	BeaconLocation = {(ESteamPartyBeaconLocation)TmpLocation.m_eType, TmpLocation.m_ulLocationID};
	return result;
}

bool USteamParties::GetBeaconDetails(FPartyBeaconID BeaconID, FSteamID& SteamIDBeaconOwner, FSteamPartyBeaconLocation& BeaconLocation, FString& Metadata) const
{
	TArray<char> TmpMeta;
	CSteamID TmpSteamID;
	SteamPartyBeaconLocation_t TmpBeaconLocation;
	bool bResult = SteamParties()->GetBeaconDetails(BeaconID, &TmpSteamID, &TmpBeaconLocation, TmpMeta.GetData(), 8192);  // Might not be the smartest to hardcode
	Metadata = UTF8_TO_TCHAR(TmpMeta.GetData());
	SteamIDBeaconOwner = TmpSteamID.ConvertToUint64();
	BeaconLocation = TmpBeaconLocation;
	return bResult;
}

bool USteamParties::GetBeaconLocationData(FSteamPartyBeaconLocation BeaconLocation, ESteamPartyBeaconLocationData_ LocationData, FString& DataString) const
{
	TArray<char> TmpData;
	bool bResult = SteamParties()->GetBeaconLocationData(BeaconLocation, (ESteamPartyBeaconLocationData)LocationData, TmpData.GetData(), 8192);  // Might not be the smartest to hardcode
	DataString = UTF8_TO_TCHAR(TmpData.GetData());
	return bResult;
}

void USteamParties::OnJoinParty(JoinPartyCallback_t* pParam)
{
	m_OnJoinParty.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_ulBeaconID, pParam->m_SteamIDBeaconOwner.ConvertToUint64(), UTF8_TO_TCHAR(pParam->m_rgchConnectString));
}

void USteamParties::OnCreateBeacon(CreateBeaconCallback_t* pParam)
{
	m_OnCreateBeacon.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_ulBeaconID);
}

void USteamParties::OnReservationNotification(ReservationNotificationCallback_t* pParam)
{
	m_OnReservationNotification.Broadcast(pParam->m_ulBeaconID, pParam->m_steamIDJoiner.ConvertToUint64());
}

void USteamParties::OnChangeNumOpenSlots(ChangeNumOpenSlotsCallback_t* pParam)
{
	m_OnChangeNumOpenSlots.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamParties::OnAvailableBeaconLocationsUpdated(AvailableBeaconLocationsUpdated_t* pParam)
{
	m_OnAvailableBeaconLocationsUpdated.Broadcast();
}

void USteamParties::OnActiveBeaconsUpdated(ActiveBeaconsUpdated_t* pParam)
{
	m_OnActiveBeaconsUpdated.Broadcast();
}

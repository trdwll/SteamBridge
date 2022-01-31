// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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
	TmpArray.Reserve(SteamDefs::Buffer128);
	int32 Num = 0;
	bool bResult = SteamParties()->GetAvailableBeaconLocations(TmpArray.GetData(), Num);
	TmpArray.SetNum(Num);
	LocationList.Reserve(TmpArray.Num());
	for (const auto& Beacon : TmpArray)
	{
		LocationList.Add(Beacon);
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
	TmpMeta.Reserve(SteamDefs::Buffer8192);
	CSteamID TmpSteamID;
	SteamPartyBeaconLocation_t TmpBeaconLocation;
	bool bResult = SteamParties()->GetBeaconDetails(BeaconID, &TmpSteamID, &TmpBeaconLocation, TmpMeta.GetData(), SteamDefs::Buffer8192);
	Metadata = UTF8_TO_TCHAR(TmpMeta.GetData());
	SteamIDBeaconOwner = TmpSteamID.ConvertToUint64();
	BeaconLocation = TmpBeaconLocation;
	return bResult;
}

bool USteamParties::GetBeaconLocationData(FSteamPartyBeaconLocation BeaconLocation, ESteamPartyBeaconLocationData_ LocationData, FString& DataString) const
{
	TArray<char> TmpData;
	TmpData.Reserve(SteamDefs::Buffer8192);
	bool bResult = SteamParties()->GetBeaconLocationData(BeaconLocation, (ESteamPartyBeaconLocationData)LocationData, TmpData.GetData(), SteamDefs::Buffer8192);
	DataString = UTF8_TO_TCHAR(TmpData.GetData());
	return bResult;
}

void USteamParties::OnJoinParty(JoinPartyCallback_t* pParam)
{
	OnJoinPartyDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_ulBeaconID, pParam->m_SteamIDBeaconOwner.ConvertToUint64(), UTF8_TO_TCHAR(pParam->m_rgchConnectString));
}

void USteamParties::OnCreateBeacon(CreateBeaconCallback_t* pParam)
{
	OnCreateBeaconDelegate.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_ulBeaconID);
}

void USteamParties::OnReservationNotification(ReservationNotificationCallback_t* pParam)
{
	OnReservationNotificationDelegate.Broadcast(pParam->m_ulBeaconID, pParam->m_steamIDJoiner.ConvertToUint64());
}

void USteamParties::OnChangeNumOpenSlots(ChangeNumOpenSlotsCallback_t* pParam)
{
	OnChangeNumOpenSlotsDelegate.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamParties::OnAvailableBeaconLocationsUpdated(AvailableBeaconLocationsUpdated_t* pParam)
{
	OnAvailableBeaconLocationsUpdatedDelegate.Broadcast();
}

void USteamParties::OnActiveBeaconsUpdated(ActiveBeaconsUpdated_t* pParam)
{
	OnActiveBeaconsUpdatedDelegate.Broadcast();
}
